#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <iostream>
#include <future>

class ThreadPool {
public:
    ThreadPool(size_t numThreads) {
        start(numThreads);
    }

    ~ThreadPool() {
        stop();
    }

    template<class T>
    void enqueue(T task) {
        {
            std::unique_lock<std::mutex> lock(mEventMutex);
            mTasks.emplace(std::make_shared<std::packaged_task<void()>>(std::move(task)));
        }
        mEventVar.notify_one();
    }

private:
    std::vector<std::thread> mThreads;
    std::condition_variable mEventVar;
    std::mutex mEventMutex;
    bool mStopping = false;
    std::queue<std::shared_ptr<std::packaged_task<void()>>> mTasks;

    void start(size_t numThreads) {
        for (auto i = 0u; i < numThreads; ++i) {
            mThreads.emplace_back([=] {
                while (true) {
                    std::shared_ptr<std::packaged_task<void()>> task;
                    {
                        std::unique_lock<std::mutex> lock(mEventMutex);
                        mEventVar.wait(lock, [=] { return mStopping || !mTasks.empty(); });
                        if (mStopping && mTasks.empty())
                            break;
                        task = std::move(mTasks.front());
                        mTasks.pop();
                    }
                    (*task)();
                }
            });
        }
    }

    void stop() noexcept {
        {
            std::unique_lock<std::mutex> lock(mEventMutex);
            mStopping = true;
        }
        mEventVar.notify_all();
        for (auto &thread : mThreads)
            thread.join();
    }
};
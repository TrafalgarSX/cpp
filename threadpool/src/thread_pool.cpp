#include <iostream>
#include <thread_pool.h>

ThreadPool::ThreadPool(int num_thread, const char* name)
   : threads_(num_thread), running_(true), work_complete_(true), started_(false), active_threads_(0) {
    assert(num_thread > 0);

    for (int i = 0; i < num_thread; ++i) {
      threads_[i] = std::thread(std::bind(&ThreadPool::ThreadMain, this, i, make_string("[LAKE][TP", i, "]", name)));
    }
    tl_errors_.resize(num_thread);
}

ThreadPool::~ThreadPool() {
  WaitForWork(false);

  std::unique_lock<std::mutex> lock(mutex_);
  running_ = false;
  condition_.notify_all();
  lock.unlock();

  for (auto &thread : threads_) {
    thread.join();
  }
}

void ThreadPool::AddWork(Work work, int64_t priority, bool start_immediately) {
  bool started_before = false;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    work_queue_.push({priority, std::move(work)});
    work_complete_ = false;
    started_before = started_;
    started_ |= start_immediately;
  }
  if (started_) {
    if (!started_before)
      condition_.notify_all();
    else
      condition_.notify_one();
  }
}

void ThreadPool::WaitForWork(bool checkForErrors) {
  std::unique_lock<std::mutex> lock(mutex_);
  completed_.wait(lock, [this] { return this->work_complete_; });
  started_ = false;
  if (checkForErrors) {
    // Check for errors
    for (size_t i = 0; i < threads_.size(); ++i) {
      if (!tl_errors_[i].empty()) {
        // Throw the first error that occurred
        std::string error = make_string("Error in thread ", i, ": ", tl_errors_[i].front());
        tl_errors_[i].pop();
        throw std::runtime_error(error);
      }
    }
  }
}

void ThreadPool::RunAll(bool wait) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    started_ = true;
  }
  condition_.notify_all();  // other threads will be waken up if needed
  if (wait) {
    WaitForWork();
  }
}

int ThreadPool::NumThreads() const {
  return threads_.size();
}

std::vector<std::thread::id> ThreadPool::GetThreadIds() const {
  std::vector<std::thread::id> tids;
  tids.reserve(threads_.size());
  for (const auto &thread : threads_)
    tids.emplace_back(thread.get_id());
  return tids;
}

void SetThreadNameInternal(const char *name) {
  char tmp_name[16];
  int i = 0;
  while (name[i] != '\0' && i < 15) {
    tmp_name[i] = name[i];
    ++i;
  }
  tmp_name[i] = '\0';
  pthread_setname_np(pthread_self(), tmp_name);
}

void SetThreadName(const char *name) {
  SetThreadNameInternal(name);
}

void ThreadPool::ThreadMain(int thread_id, const std::string &name) {
  SetThreadName(name.c_str());

  while (running_) {
    // Block on the condition to wait for work
    std::unique_lock<std::mutex> lock(mutex_);
    condition_.wait(lock, [this] { return !running_ || (!work_queue_.empty() && started_); });
    // If we're no longer running, exit the run loop
    if (!running_) break;

    // Get work from the queue & mark
    // this thread as active
    Work work = std::move(work_queue_.top().second);
    work_queue_.pop();
    ++active_threads_;

    // Unlock the lock
    lock.unlock();

    // If an error occurs, we save it in tl_errors_. When
    // WaitForWork is called, we will check for any errors
    // in the threads and return an error if one occured.
    try {
      work(thread_id);
    } catch (std::exception &e) {
      lock.lock();
      tl_errors_[thread_id].push(e.what());
      lock.unlock();
    } catch (...) {
      lock.lock();
      tl_errors_[thread_id].push("Caught unknown exception");
      lock.unlock();
    }

    // Mark this thread as idle & check for complete work
    lock.lock();
    --active_threads_;
    if (work_queue_.empty() && active_threads_ == 0) {
      work_complete_ = true;
      lock.unlock();
      completed_.notify_one();
    }
  }
}
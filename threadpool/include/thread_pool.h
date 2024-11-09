#include <cstdlib>
#include <utility>
#include <condition_variable>
#include <functional>
#include <vector>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <cassert>
#include <atomic>
#include <sstream>
#include <iostream>


class ThreadPool {
public:
  typedef std::function<void(int)> Work;
  ThreadPool(int num_thread, const char* name);

  ThreadPool(int num_thread, const std::string& name)
    : ThreadPool(num_thread, name.c_str()) {}

  ~ThreadPool();
  void AddWork(Work work, int64_t priority = 0, bool start_immediately = false);

  void RunAll(bool wait = true);

  void WaitForWork(bool checkForErrors = true);

  int NumThreads() const;

  std::vector<std::thread::id> GetThreadIds() const;

private:
  void ThreadMain(int thread_id, const std::string &name);
  std::vector<std::thread> threads_;

  using PrioritizedWork = std::pair<int64_t, Work>;

  struct SortByPriority {
    bool operator() (const PrioritizedWork &a, const PrioritizedWork &b) {
      return a.first < b.first;
    }
  };
  std::priority_queue<PrioritizedWork, std::vector<PrioritizedWork>, SortByPriority> work_queue_;

  bool running_;
  bool work_complete_;
  bool started_;
  int active_threads_;
  std::mutex mutex_;
  std::condition_variable condition_;
  std::condition_variable completed_;

  //  Stored error strings for each thread
  std::vector<std::queue<std::string>> tl_errors_;

};

struct no_delimiter {};

inline std::ostream &operator<<(std::ostream &os, no_delimiter) { return os; }

template <typename Collection, typename Delimiter>
std::ostream &join(std::ostream &os, const Collection &collection, const Delimiter &delim) {
  bool first = true;
  for (auto &element : collection) {
    if (!first) {
      os << delim;
    } else {
      first = false;
    }
    os << element;
  }
  return os;
}

template <typename Collection>
std::ostream &join(std::ostream &os, const Collection &collection) {
  return join(os, collection, ", ");
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
  return join(os, vec);
}

template <typename T, size_t N>
std::ostream &operator<<(std::ostream &os, const std::array<T, N> &arr) {
  return join(os, arr);
}


template <typename Delimiter>
void print_delim(std::ostream &os, const Delimiter &delimiter) {
  // No-op
}


template <typename Delimiter, typename T>
void print_delim(std::ostream &os, const Delimiter &delimiter, const T &val) {
  os << val;
}

/**
 * @brief Populates stream with given arguments, as long as they have
 * `operator<<` defined for stream operation
 */
template <typename Delimiter, typename T, typename... Args>
void print_delim(std::ostream &os, const Delimiter &delimiter, const T &val,
                 const Args &... args) {
  os << val << delimiter;
  print_delim(os, delimiter, args...);
}

/**
 * @brief Populates stream with given arguments, as long as they have
 * `operator<<` defined for stream operation
 */
template <typename... Args>
void print(std::ostream &os, const Args &... args) {
  print_delim(os, no_delimiter(), args...);
}

/**
 * Creates std::string from arguments, as long as every element has `operator<<`
 * defined for stream operation.
 *
 * If there's no `operator<<`, compiler will fire an error
 *
 * @param delimiter String, which will separate arguments in the final string
 * @return Concatenated std::string
*/
template <typename Delimiter, typename... Args>
std::string make_string_delim(const Delimiter &delimiter, const Args &... args) {
  std::stringstream ss;
  print_delim(ss, delimiter, args...);
  return ss.str();
}


/**
 * This overload handles the edge case when no arguments are given and returns an empty string
 */
template <typename Delimiter>
std::string make_string_delim(const Delimiter &) {
  return {};
}

/**
 * @brief Prints args to a string, without any delimiter
 */
template <typename... Args>
std::string make_string(const Args &... args) {
  std::stringstream ss;
  print(ss, args...);
  return ss.str();
}


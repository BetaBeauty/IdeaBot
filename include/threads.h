#ifndef THREADS_H
#define THREADS_H

#include <thread>
#include <vector>
#include <atomic>
#include <unordered_map>

namespace bot {
namespace thread {

struct Thread {
  std::thread handle;
  std::atomic<bool> is_run;
};

class ThreadManager {
 public:
  static ThreadManager& Global() {
    static ThreadManager ins;
    return ins;
  }

  template<typename ...Args>
  void RegisterThread(Args... args) {
    std::thread tid(args...);
    // thread_entrys.emplace_back(tid);
  }

  std::atomic<bool> exit;

  void ExitAllThreads() {
  }

 private:
  std::vector<Thread> thread_entrys;
  std::unordered_map<std::thread::id, Thread> thread_indexes;
};

#define THREAD_MANAGER ThreadManager::Global()
    
}
}

#endif // THREADS_H

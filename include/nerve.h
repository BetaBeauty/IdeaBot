#ifndef NERVE_H
#define NERVE_H

#include <iostream>
#include <sstream>
#include <queue>
#include <mutex> 
#include <atomic>
#include <condition_variable>
#include <thread>

#include "base.h"
#include "core.h"

namespace bot{
namespace inner {

template<
  template<typename> class DevType,
  typename DType,
  typename... Args>
inline std::shared_ptr<DevType<DType>> Make(Args... args) {
  return std::make_shared<DevType<DType>>(args...);
}

template<typename DType>
using CallFunc = std::function<void(DType)>;

template<typename DType>
class Dendrite {
 public:
  Dendrite(CallFunc<DType> &func) : call(func) {}

  CallFunc<DType> call;
};

template<typename DType>
using DdrPtr = std::shared_ptr<Dendrite<DType>>;

enum Status {
  INIT,
  READY,
  START,
  DESTORY
}; 

template<typename DType = std::string>
class Nerve {
 public:
  static std::shared_ptr<Nerve<DType>> Make() {
    return std::make_shared<Nerve<DType>>();
  }

  Nerve() : _status(Status::INIT), _ddr(nullptr) {
    std::thread t(&Nerve::start, this);
    t.detach();
  }

  ~Nerve() {
    destroy();
  }

  Nerve& send(DType val) {
    std::unique_lock<std::mutex> lock(_mutex);
    _buffer.emplace_back(std::move(val));
    lock.unlock();

    _cv.notify_all();
    return *this;
  }

  void link_dendrite(DdrPtr<DType> ddr) {
    _ddr = ddr;
    _status.store(Status::READY);
  } 

 private:
  void start() {
    std::unique_lock<std::mutex> lock(_mutex);
    while (_status != Status::DESTORY) {
      _cv.wait(lock, [this]() {
        return _buffer.size() || _status == Status::DESTORY;
      });

      if (_status == Status::DESTORY) return ;
      
      std::vector<DType> bak;
      _buffer.swap(bak);
      lock.unlock();

      for (auto & val : bak) {
        if (_ddr) _ddr->call(val);
      }
    }
  }

  void destroy() {
    _status.store(Status::DESTORY);
    _cv.notify_all();
  }

 private:
  std::atomic<int> _status;

  std::condition_variable _cv;
  std::mutex _mutex;
  std::vector<DType> _buffer;

  DdrPtr<DType> _ddr;
};

template<typename DType>
using NervePtr = std::shared_ptr<Nerve<DType>>;

}

namespace device {

template<typename DType>
class Device {
 public:
  virtual inner::NervePtr<DType>
  link_core(inner::NervePtr<DType>) = 0;
};

template<typename DType>
using DevPtr = std::shared_ptr<Device<DType>>;

template<typename DevType, typename... Args>
inline std::shared_ptr<DevType> Make(Args... args) {
  return std::make_shared<DevType>(args...);
}

}

}

#endif // NERVE_H

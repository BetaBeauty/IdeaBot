#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <memory>
#include <functional>
#include <thread>

namespace bot {
namespace device {

using CallbackFunc = std::function<void(std::string &&)>;

class Stream {
 public:
  void SetCallback(CallbackFunc && func) {
    this->callback = func;
  }

  void SetCallback(CallbackFunc & func) {
    this->callback = func;
  }

  Stream& Push(std::string val) {
    if (this->callback) {
      // Invoke callback asynchronize to avoid block.
      std::thread t(this->callback, std::move(val));
      t.detach();
    }
    return *this;
  }

 protected:
  CallbackFunc callback;
};

class DeviceRef {
 public:
  virtual bool LinkCallStream(Stream & os) noexcept = 0;
  virtual bool LinkBackStream(Stream & is) noexcept = 0;
  virtual bool Destroy() noexcept = 0;
};

using DevicePtr = std::shared_ptr<DeviceRef>;

template<class Device>
inline DevicePtr Connect() {
  return std::make_shared<Device>();
}

}
}

#endif // DEVICE_H

#ifndef DEVICE_H
#define DEVICE_H

#include <iostream>
#include <memory>
#include <functional>
#include <thread>
#include <unordered_map>

#include "nerve.h"

namespace bot {
namespace device {


class _DeviceBase {
 public:
  void registry();
  void destory();
  std::size_t hash() {
    return reinterpret_cast<size_t>(this);
  }
};

using _DevBasePtr = std::shared_ptr<_DeviceBase>;

template<typename DType>
class Device : public _DeviceBase {
 public:
  virtual inner::NervePtr<DType>
  link_core(inner::NervePtr<DType>) = 0;

  inner::NervePtr<DType> bind_dentrite() {
    std::function<void(DType)> f = [this](DType val) {
      process(val);
    };

    auto dendrite = inner::Make<inner::Dendrite, DType>(f);
    auto nerve = inner::Make<inner::Nerve, DType>();
    nerve->link_dendrite(dendrite);
    return nerve;
  }

  virtual void process(DType val) = 0;
};

template<typename DType>
using DevPtr = std::shared_ptr<Device<DType>>;

class DeviceManager {
 public:
  static DeviceManager& Global() {
    static DeviceManager ins;
    return ins;
  }

  void __REGISTER__(_DevBasePtr ref) {
    _entrys[ref->hash()] = ref;
  }

 private:
  std::unordered_map<size_t, _DevBasePtr> _entrys;

};

template<typename DevType, typename... Args>
inline std::shared_ptr<DevType> Make(Args... args) {
  auto ref = std::make_shared<DevType>(args...);
  DeviceManager::Global().__REGISTER__(ref);
  return ref;
}

}
}

#endif // DEVICE_H

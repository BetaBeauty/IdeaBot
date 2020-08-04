#ifndef CORE_H
#define CORE_H

#include <vector>

#include "nerve.h"
#include "device/intergral.h"

namespace bot {
namespace core {

class Soul {
 public:

};

class Core {
 public:
  Core() {
    link_ear();
    link_mouth();
  }

  void start_mind() {
    std::unique_lock<std::mutex> lock(_mutex);
    while (true) {
      _cv.wait(lock);

    }
  }

 private:
  void link_ear() {
    std::function<void(std::string)> f = [](std::string val) {
      
    };
    _ear = inner::Make<inner::Nerve, std::string>();
    _ear->link_dendrite(inner::Make<inner::Dendrite, std::string>(f));

    auto keyboard = device::Make<device::Keyboard>();
    keyboard->link_core(_ear);
  }

  void link_mouth() {
    std::function<void(std::string)> f = [](std::string val) {
    
    };
    _mouth = inner::Make<inner::Nerve, std::string>();
    _mouth->link_dendrite(
        inner::Make<inner::Dendrite, std::string>(f));
    
    auto display = device::Make<device::Display>();
    display->link_core(_mouth);
  }

  std::mutex _mutex;
  std::condition_variable _cv;

  inner::NervePtr<std::string> _ear;
  inner::NervePtr<std::string> _mouth;
};


}
}

#endif // CORE_H

#ifndef INTERGRAL_H
#define INTERGRAL_H

#include <functional>

#include "../device.h"
#include "../nerve.h"

namespace bot {
namespace device {

class Display : public Device<std::string> {
  using DType = std::string;
 public:
  Display() {
    _call_nerve = bind_dentrite();
  }

  void process(std::string val) override {
    std::cout << "[display]: " << val;
    std::flush(std::cout);
  }

  inner::NervePtr<std::string>
  link_core(inner::NervePtr<std::string> nerve) override {
    _back_nerve = nerve;
    return _call_nerve;
  }

  inner::NervePtr<std::string> _call_nerve;
  inner::NervePtr<std::string> _back_nerve;
};

class Keyboard : public Device<std::string> {
  using DType = std::string;
 public:
  Keyboard() : _status(inner::Status::INIT) {
    _call_nerve = bind_dentrite();
  }

  void process(std::string val) override {
    // Keyboard not handle the call method.
    std::cout << "[Keyboard]: ignored " << val << "\n";
    std::flush(std::cout);
  }

  inner::NervePtr<std::string>
  link_core(inner::NervePtr<std::string> nerve) override {
    if (_status != inner::Status::READY) {
      _listen_thread = 
        std::thread(&Keyboard::listenKeyboard, this);
      _status = inner::Status::READY;
    }
    _back_nerve = nerve;
    return _call_nerve;
  }

  ~Keyboard() {
    _status = inner::Status::DESTORY;
    if (_listen_thread.joinable()) {
      _listen_thread.join();
    }
  }

 private:
  void listenKeyboard() {
    static thread_local std::string val;

    while (_status != inner::Status::DESTORY) {
      if (std::cin.peek() != EOF) {
        std::cout << "[wait for input " << std::cin.peek() << "]: ";
        std::cin >> val;
        _back_nerve->send(val);
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
    }
  }

  inner::Status _status;
  std::thread _listen_thread;
  inner::NervePtr<std::string> _call_nerve;
  inner::NervePtr<std::string> _back_nerve;
};

}
}

#endif // INTERGRAL_H

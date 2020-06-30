#ifndef INTERGRAL_H
#define INTERGRAL_H

#include <functional>

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

class Terminal : public Device<std::string> {
  using DType = std::string;
 public:
  Terminal() : interrupt(0) {
    _call_nerve = bind_dentrite();
    _status = inner::Status::INIT;

    std::thread t(&Terminal::listenInputStream, this);
    t.detach();
  }

  void process(std::string val) override {
    // Terminal not handle the call method.
    std::cout << "[Terminal]: ignored " << val << "\n";
    std::flush(std::cout);
  }

  inner::NervePtr<std::string>
  link_core(inner::NervePtr<std::string> nerve) override {
    if (_status != inner::Status::READY) {
      _listen_thread[0] = 
        std::thread(&Terminal::listenInputStream, this);
      _status = inner::Status::READY;
    }
    _back_nerve = nerve;
    return _call_nerve;
  }

  ~Terminal() {
    _status = inner::Status::DESTORY;
    if (_listen_thread[0].joinable()) {
      _listen_thread[0].join();
    }
  }

 private:
  void listenInputStream() {
    static thread_local std::string val;

    while (_status != inner::Status::DESTORY) {
      if (std::cin.peek() != EOF) {
        std::cin >> val;
        _back_nerve->send(val);
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
    }
  }

  inner::Status _status;
  std::thread _listen_thread[1];
  std::atomic<int8_t> interrupt;
  inner::NervePtr<std::string> _call_nerve;
  inner::NervePtr<std::string> _back_nerve;
};

}
}

#endif // INTERGRAL_H

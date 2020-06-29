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
    std::function<void(std::string)> f = std::bind(
        &Display::process, this,
        std::placeholders::_1);
    _dendrite = inner::Make<inner::Dendrite, DType>(f);
    _call_nerve = inner::Make<inner::Nerve, DType>();
    _call_nerve->link_dendrite(_dendrite);
  }

  void process(std::string val) {
    std::cout << "[display]: " << val;
    std::flush(std::cout);
  }

  inner::NervePtr<std::string>
  link_core(inner::NervePtr<std::string> nerve) override {
    _back_nerve = nerve;
    return _call_nerve;
  }

  inner::DdrPtr<std::string> _dendrite;
  inner::NervePtr<std::string> _call_nerve;
  inner::NervePtr<std::string> _back_nerve;
};

class Terminal : public Device<std::string> {
  using DType = std::string;
 public:
  Terminal() {
    std::function<void(std::string)> f = std::bind(
        &Terminal::process, this,
        std::placeholders::_1);
    _dendrite = inner::Make<inner::Dendrite, DType>(f);
    _call_nerve = inner::Make<inner::Nerve, DType>();
    _call_nerve->link_dendrite(_dendrite);
  }

  void process(std::string val) {
    
  }

  inner::NervePtr<std::string>
  link_core(inner::NervePtr<std::string> nerve) override {
    _back_nerve = nerve;
    return _call_nerve;
  }

 private:
  void listenInputStream() {
    static thread_local std::string val;

    // while (!interrupt) {
      // if (std::cin.peek() != EOF) {
        // std::cin >> val;
        // back_stream.Push(val);
      // } else {
        // std::this_thread::sleep_for(std::chrono::milliseconds(200));
      // }
    // }

    // thread_status = false;
  }

  inner::DdrPtr<std::string> _dendrite;
  inner::NervePtr<std::string> _call_nerve;
  inner::NervePtr<std::string> _back_nerve;
};

}
}

#endif // INTERGRAL_H

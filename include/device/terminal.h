#ifndef DEV_TERMINAL_H
#define DEV_TERMINAL_H

#include <atomic>
#include <thread>
#include <chrono>

#include "../device.h"

namespace bot {
namespace device {

class Display : public DeviceRef {
 public:
  bool LinkCallStream(Stream & stream) noexcept override {
    std::cout << "[Device:Display] Link call stream\n";
    stream.SetCallback([](std::string && val) {
      std::cout << "[" << val << "]" << std::endl;
    });
    this->os = &stream;

    return true;
  }

  bool LinkBackStream(Stream & os) noexcept override {
    std::cerr << "[Device:Display] No back stream\n";
    return false;
  }

  bool Destroy() noexcept override {
    this->os->SetCallback(nullptr);
    return true;
  }

 private:
  Stream * os;
};

class Keyboard : public DeviceRef {
 public:
  Keyboard() : interrupt(false), thread_status(false) {}

  bool LinkCallStream(Stream & os) noexcept override {
    std::cerr << "Keyboard has not call stream\n";
    return false;
  }

  bool LinkBackStream(Stream & os) noexcept override {
    back_stream = os;

    if (thread_status) {
      std::cerr << "Keyboard has linked the back stream\n";
      return false;
    }

    thread_status = true;
    std::thread listen_thread(&Keyboard::listenInputStream, this);

    return true;
  }

  bool Destroy() noexcept override {
    interrupt.store(true); 
    return true;
  }

 private:
  void listenInputStream() {
    static thread_local std::string val;

    while (!interrupt) {
      if (std::cin.peek() != EOF) {
        std::cin >> val;
        back_stream.Push(val);
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
    }

    thread_status = false;
  }

  Stream back_stream;
  std::atomic<bool> interrupt, thread_status;

};

}
}

#endif // DEV_TERMINAL_H

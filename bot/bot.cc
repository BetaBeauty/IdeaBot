#include <iostream>

#include <device.h>
#include <device/intergral.h>

using namespace bot;

int main() {
  std::cout << "Idea Bot Starting" << std::endl;

  // auto display = device::Make<device::Display>();
  // auto nerve = inner::Make<inner::Nerve, std::string>();
  // auto call = display->link_core(nerve);
  // std::cout << "Start" << std::endl;
  // call->send("test test my call")
    // .send("\n")
    // .send("net line");

  std::function<void(std::string)> f = [](std::string val) {
    std::cout << "[core]: " << val << "\n";
    std::flush(std::cout);
  };
  auto nerve = inner::Make<inner::Nerve, std::string>();
  nerve->link_dendrite(inner::Make<inner::Dendrite, std::string>(f));

  {
    auto keyboard = device::Make<device::Keyboard>();
    auto call = keyboard->link_core(nerve);
    std::cout << "start" << std::endl;
    call->send("try to send\n");
  }


  std::this_thread::sleep_for(std::chrono::seconds(10));

  // auto display = device::Connect<device::Display>();

  // device::Stream cin;
  // std::cout << display.get() << "\n";
  // display->LinkCallStream(cin);
  // cin.Push("test test my call")
    // .Push("\n")
    // .Push("next line");

}

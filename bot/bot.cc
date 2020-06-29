#include <iostream>

#include <device.h>
// #include <device/terminal.h>
#include <device/intergral.h>

using namespace bot;

int main() {
  std::cout << "Idea Bot Starting" << std::endl;

  auto display = device::Make<device::Display>();
  auto nerve = inner::Make<inner::Nerve, std::string>();
  auto call = display->link_core(nerve);
  std::cout << "Start" << std::endl;
  call->send("test test my call")
    .send("\n")
    .send("net line");


  std::this_thread::sleep_for(std::chrono::seconds(5));

  // auto display = device::Connect<device::Display>();

  // device::Stream cin;
  // std::cout << display.get() << "\n";
  // display->LinkCallStream(cin);
  // cin.Push("test test my call")
    // .Push("\n")
    // .Push("next line");

}

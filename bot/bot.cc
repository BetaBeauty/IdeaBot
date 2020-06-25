#include <iostream>

#include <device.h>
#include <device/terminal.h>

using namespace bot;

int main() {
  std::cout << "Idea Bot Starting" << std::endl;

  auto display = device::Connect<device::Display>();

  device::Stream cin;
  std::cout << display.get() << "\n";
  display->LinkCallStream(cin);
  cin.Push("test test my call")
    .Push("\n")
    .Push("next line");

}

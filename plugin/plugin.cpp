#include <iostream>

extern "C" {
  void sayHello() {
    std::cout << "Hello from plugin!\n";
  }

  bool init() {
	  std::cout << "Init\n";
	  return true;
  }

  void deinit() {
	std::cout << "Deinit\n";
  }
  void run() {
	std::cout << "Run\n";
  }
}

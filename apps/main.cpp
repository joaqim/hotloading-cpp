/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file is part of the musl_test project:
 *     https://github.com/radupopescu/musl_test
 */

#include "musl_test_config.h"

#include <dummy.h>

#include <iostream>
#include <future>
#include <thread>

#include <dlfcn.h>

typedef void(*PluginFunction)(void);
typedef void(*FUNC)(void);
typedef bool(*FUNC_B)(void);

struct library {
	library() = default;
	void *pLibrary = nullptr;
	bool initialized = false;

  FUNC_B init;
  FUNC deinit;
  FUNC run;
};

library loadLibrary(const char* filename) {
	library lib;
	lib.pLibrary = dlopen(filename, RTLD_LAZY);
	if (lib.pLibrary == nullptr) {
		std::cerr << "Error loading plugin." << std::endl;
  }

	return lib;
};

void unloadLibrary(library lib) {
  dlclose(lib.pLibrary);
  lib.initialized = false;
}

int main()
{
	// C++11 std::thread
	auto t1 = std::thread([] {
			std::cout << "musl_test version: " << musl_test_VERSION_STRING << std::endl;
			});

	t1.join();

	const auto n = 1;

	// C++14 generic lambda
	auto f = [n] (auto i) {
		return n + i;
	};

	std::cout << "f(1) = " << f(1) << std::endl;

	// C++11 std::async
	auto result = std::async(f, 2);

	std::cout << "f(2) = " << result.get() << std::endl;

	auto lib = loadLibrary("../plugin/libmusl_test_plugin.so");

  lib.init = (FUNC_B)dlsym(lib.pLibrary, "init");
  lib.deinit = (FUNC)dlsym(lib.pLibrary, "deinit");
  lib.run = (FUNC)dlsym(lib.pLibrary, "run");

  if(lib.init()) {
    lib.initialized = true;
    lib.deinit();
    lib.run();
  }


  unloadLibrary(lib);

  return 0;
}

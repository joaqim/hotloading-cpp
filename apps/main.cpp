
#include <CommonTypes.h>
#include <Log.h>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

#include <iostream>
#include <sys/mman.h> // nmap, unmap, mprotect
#include <sys/stat.h> // open() dep
#include <fcntl.h> // O_RDONLY etc
#include <unistd.h> // close()
#include <stdlib.h> // free()
#include <dlfcn.h>


typedef void(*FUNC_V)(void);
typedef bool32(*FUNC_B)(void);

struct Plugin {
	Plugin() = default;
	void *pLibrary = nullptr;
	bool32 isInitialized = false;
	
	FUNC_B init;
	FUNC_V deinit;
	FUNC_V run;
};


template<typename T>
T load(const char* filename) {
  T lib;
  lib.pLibrary = dlopen(filename, RTLD_LAZY);
  if (lib.pLibrary == nullptr) {
    std::cerr << "Error loading plugin: " << errno << "\n";
  } else {
    lib.init =   (FUNC_B)dlsym(lib.pLibrary, "init");
    lib.deinit = (FUNC_V)dlsym(lib.pLibrary, "deinit");
    lib.run   =  (FUNC_V)dlsym(lib.pLibrary, "run");
#if 0
    if (lib.init()) {
    	lib.isInitialized = true;
    } else {
        std::cerr << "Error initializing plugin: " << errno << "\n";
    }
#endif
  }
  return lib;
};

template <typename T>
void unload(T lib) {
  dlclose(lib.pLibrary);
  lib.isInitialized = false;
}

using namespace Magnum;

class MyApplication: public Platform::Application {
  typedef Terrific::Utility::Log Log;
 public:
  explicit MyApplication(const Arguments &arguments);
  ~MyApplication();
 private:
  void drawEvent() override;
  void keyPressEvent(KeyEvent &event) override;
 private:
  Log _log;
};

void MyApplication::keyPressEvent(KeyEvent &event) {
	if(event.key() == KeyEvent::Key::Q)
		exit();
}

MyApplication::MyApplication(const Arguments &arguments): Platform::Application{arguments} {
        using Terrific::Utility::Log;
        auto log = Log::init();
	auto plugin = load<Plugin>("../plugin/libmusl_test_plugin_plugin.so");
	//Assert(plugin.isInitialized);
	plugin.init();
}

void MyApplication::drawEvent() {
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
	swapBuffers();
}

MyApplication::~MyApplication() {
}

MAGNUM_APPLICATION_MAIN(MyApplication)
#if 0
int main() {
  using Terrific::Utility::Log;
  auto log = Log::init();

  return 0;
}
#endif

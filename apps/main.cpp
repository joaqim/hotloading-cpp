
#include <CommonTypes.h>
#include <Log.h>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>

#include <sys/mman.h> // nmap, unmap, mprotect

using namespace Magnum;

class MyApplication: public Platform::Application {
 public:
  explicit MyApplication(const Arguments &arguments);
  ~MyApplication();
 private:
  void drawEvent() override;
  void keyPressEvent(KeyEvent &event) override;
 private:
};

void MyApplication::keyPressEvent(KeyEvent &event) {
	if(event.key() == KeyEvent::Key::Q)
		exit();
}

MyApplication::MyApplication(const Arguments &arguments): Platform::Application{arguments} {
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

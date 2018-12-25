
#include <CommonTypes.h>
#include <Log.h>

#include <sys/mman.h> // nmap, unmap, mprotect

int main() { 
  using Terrific::Utility::Log;
  auto log = Log::init();

  return 0;
}

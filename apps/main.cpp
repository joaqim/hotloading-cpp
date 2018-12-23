
#include <CommonTypes.h>
#include <Log.h>

#include <CodeLoader.h>

#include <sys/mman.h> // nmap, unmap, mprotect

int main() { 
  using Terrific::Utility::Log;
  auto log = Log::init();


  GameMemory gameMemory{Megabytes(8), Megabytes(4)};

  return 0;
}

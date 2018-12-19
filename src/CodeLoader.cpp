
#include "CodeLoader.h"
#include "Log.h"

#include <unix_game.h>

#include <iostream>
#include <dlfcn.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h> // struct stat, for open()
#include <fcntl.h>

File GameLoader::read(const char *filename) {
  File result = {};
  int fileHandle = open(filename, O_RDONLY);
  if (fileHandle == -1) {
    return result;
  }


  struct stat fileStatus;
  if (fstat(fileHandle, &fileStatus) == -1) {
    close(fileHandle);
    return result;
  }
  result.size = SafeTruncateUInt64(fileStatus.st_size);

  result.pContents = malloc(result.size);
  if (!result.pContents) {
    result.size = 0;
    close(fileHandle);
    return result;
  }

  uint32 BytesToRead = result.size;
  uint8 *NextByteLocation = (uint8*)result.pContents;
  while (BytesToRead) {
    uint32 BytesRead = read(fileHandle, NextByteLocation, BytesToRead);
    if (BytesRead == -1) {
      free(result.pContents);
      result.pContents = 0;
      result.size = 0;
      close(fileHandle);
      return result;
    }
     BytesToRead -= BytesRead;
     NextByteLocation += BytesRead;
   }
  close(fileHandle);

  return result;
}
void GameLoader::write(const char *filename, const void *memory, const uint32 size) {
}

GameCode GameLoader::load(const char *filename) {
  GameCode code;
  code.pLibrary = dlopen(filename, RTLD_LAZY);
  if (code.pLibrary == nullptr)
    std::cerr << "Error loading plugin: " << errno << "\n";
  return code;
}

void GameLoader::unload(GameCode &code) {
  dlclose(code.pLibrary);
  code.isValid = false;
}



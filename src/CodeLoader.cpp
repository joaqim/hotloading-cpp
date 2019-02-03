
#include "CodeLoader.h"

File CodeLoader::readFile(const char *filename) {
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
bool32 CodeLoader::writeFile(const char *filename, const void *memory, const uint32 size) {
  //Timer tim;
  //Assert(size > 0, "Memory is empty");
#if 1 // Somehow faster than fopen
  int fileHandle = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (fileHandle == -1)
    return false;

  uint32 bytesToWrite = size;
  uint8 *nextByteLocation = (uint8*)memory;
  while (bytesToWrite) {
    uint32 bytesWritten = write(fileHandle, nextByteLocation, bytesToWrite);
    if (bytesWritten == -1) {
      close(fileHandle);
      return false;
    }
    bytesToWrite -= bytesWritten;
    nextByteLocation += bytesWritten;
  }

  close(fileHandle);
#else
  FILE* file = fopen(filename, "wb");
  fwrite(memory, 1, size, file);
  fclose(file);
#endif
  //std::cout << tim << "\n";
  return true;

}
void CodeLoader::freeMemory(void *memory) {
  free(memory);
}

GameCode CodeLoader::loadCode (const char *filename) {
  GameCode code;
  code.pLibrary = dlopen(filename, RTLD_LAZY);
  if (code.pLibrary == nullptr) {
    std::cerr << "Error loading plugin: " << errno << "\n";
    code.isValid = false;
  } else {
    code.isValid = true;
    code.init = (GAME_INIT)dlsym(code.pLibrary, "game_init");
    code.deinit = (GAME_DEINIT)dlsym(code.pLibrary, "game_deinit");
    code.run = (GAME_RUN)dlsym(code.pLibrary, "game_run");
  }
  return code;
}

void CodeLoader::unloadCode(GameCode &code) {
  dlclose(code.pLibrary);
  code.isValid = false;
}



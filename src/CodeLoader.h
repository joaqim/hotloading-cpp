#pragma once
#include <CommonTypes.h>
#include <Memory.h>

//#include <Log.h>

#include <iostream>
#include <dlfcn.h>
#include <unistd.h> // read()
//#include <stdlib.h> // free()

#include <sys/types.h>
#include <sys/stat.h> // struct stat, for open()
#include <fcntl.h>

struct File {
  uint32 size;
  void *pContents;
};

struct GameCode {
  GameCode() = default;
  void *pLibrary = nullptr;
  bool32 isValid = false;

  //GAME_INIT Init(GameMemory *gameMemory);
  GAME_INIT init;
  GAME_DEINIT deinit;
  GAME_RUN run;
};

class CodeLoader {
public:
  static File readFile(const char *filename);
  static bool32 writeFile(const char *filename, const void *memory, const uint32 size);
  static void freeMemory(void *memory);
  static GameCode loadCode(const char *filename);
  static void unloadCode(GameCode &code);

private:
  GameCode _gameCode;
};

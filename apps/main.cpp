/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file is part of the musl_test project:
 *     https://github.com/radupopescu/musl_test
 */

#include "musl_test_config.h"

#include "Log.h"


#include <dummy.h>

#define _GNU_SOURCE // Needed for UNIX dlfcn.h ?
#include <iostream>

#include <future>
#include <thread>
#include <iomanip>      // std::setprecision
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h> // open() dep
#include <fcntl.h> // O_RDONLY etc
#include <unistd.h> // close()
#include <sys/mman.h> // nmap, unmap, mprotect

#include "unix_game.h"

#include <catch.hpp>
//#include <fakeit.hpp>

inline uint32
SafeTruncateUInt64(uint64 Value) {
	Assert(Value <= 0xFFFFFFFF,"");
	uint32 Result = (uint32)Value;
	return(Result);
}

internal void
DEBUGPlatformFreeFileMemory(void *Memory) {
	free(Memory);
}
// File handling
struct debug_read_file_result {
	uint32 ContentsSize;
	void *Contents;
};

internal debug_read_file_result DEBUGPlatformReadEntireFile(const char *Filename) {
  debug_read_file_result Result = {};
  int FileHandle = open(Filename, O_RDONLY);
  if (FileHandle == -1) {
    return Result;
    }


  struct stat FileStatus;
  if (fstat(FileHandle, &FileStatus) == -1) {
    close(FileHandle);
    return Result;
  }
  Result.ContentsSize = SafeTruncateUInt64(FileStatus.st_size);

  Result.Contents = malloc(Result.ContentsSize);
  if (!Result.Contents) {
    Result.ContentsSize = 0;
    close(FileHandle);
    return Result;
  }

  uint32 BytesToRead = Result.ContentsSize;
  uint8 *NextByteLocation = (uint8*)Result.Contents;
  while (BytesToRead) {
    uint32 BytesRead = read(FileHandle, NextByteLocation, BytesToRead);
    if (BytesRead == -1) {
      free(Result.Contents);
      Result.Contents = 0;
      Result.ContentsSize = 0;
      close(FileHandle);
      return Result;
    }
    BytesToRead -= BytesRead;
    NextByteLocation += BytesRead;
  }
  close(FileHandle);

  return Result;
}
class Timer
{
public:
  Timer() : beg_(clock_::now()) {}
  void reset() { beg_ = clock_::now(); }
  double elapsed() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>
      (clock_::now() - beg_).count(); }

  inline const std::string to_str() const {
    std::ostringstream stream;
    stream << std::fixed;
    stream << "Milliseconds elapsed: "
           << std::setprecision(1)
           << (float)std::chrono::duration_cast<std::chrono::milliseconds> (clock_::now() - beg_).count();
    return stream.str();
  }

private:
  typedef std::chrono::high_resolution_clock clock_;
  typedef std::chrono::duration<double, std::ratio<1> > second_;
  std::chrono::time_point<clock_> beg_;
};

template<class T>
auto operator<<(std::ostream& os, const T& t) -> decltype(t.to_str(), os) {
  return os << t.to_str();
}

internal bool32
DEBUGPlatformWriteEntireFile(const char *Filename, uint32 MemorySize, void *Memory) {
  Timer tim;
  Assert(MemorySize > 0, "Memory is empty");
#if 1 // Somehow faster than fopen
  int FileHandle = open(Filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (FileHandle == -1)
    return false;

  uint32 BytesToWrite = MemorySize;
  uint8 *NextByteLocation = (uint8*)Memory;
  while (BytesToWrite) {
    uint32 BytesWritten = write(FileHandle, NextByteLocation, BytesToWrite);
    if (BytesWritten == -1) {
      close(FileHandle);
          return false;
        }
      BytesToWrite -= BytesWritten;
      NextByteLocation += BytesWritten;
    }

  close(FileHandle);
#else
  FILE* file = fopen(Filename, "wb");
  fwrite(Memory, 1, MemorySize, file);
  fclose(file);
#endif
  std::cout << tim << "\n";
  //tim.operator<<(std::cout);
  return true;
}

internal game_state*
CreateGameState() {
	game_state *State = new game_state;
	if (State) {
    State->BlueOffset = 0;
    State->GreenOffset = 0;
    State->ToneHz = 256;
  }
	return State;
}

struct library {
  library() = default;
  void *pLibrary = nullptr;
  bool32 isInitialized = false;

  FUNC_B init;
  FUNC deinit;
  FUNC run;
};

struct game_library {
  game_library() = default;
  void *pLibrary = nullptr;
  bool32 isInitialized = false;

  GAME_INIT init;
  GAME_DEINIT deinit;
  GAME_RUN run;
};


template<typename T>
T load(const char* filename) {
  T lib;
  lib.pLibrary = dlopen(filename, RTLD_LAZY);
  if (lib.pLibrary == nullptr)
    std::cerr << "Error loading plugin: " << errno << "\n";
  return lib;
};

template <typename T>
void unload(T lib) {
  dlclose(lib.pLibrary);
  lib.isInitialized = false;
}

int main() {
  auto log = Terrific::Utility::Log::init();

  auto lib = load<library>("../plugin/libmusl_test_plugin_plugin.so");
  Assert(lib.pLibrary != nullptr,"failed to load plugin library");

  lib.init = (FUNC_B)dlsym(lib.pLibrary, "init");
  lib.deinit = (FUNC)dlsym(lib.pLibrary, "deinit");
  lib.run = (FUNC)dlsym(lib.pLibrary, "run");

  if(lib.init()) {
    lib.isInitialized = true;
    lib.deinit();
    lib.run();
  }


  unload<library>(lib);

#ifdef MY_DEBUG
  void *BaseAddress = (void *)Terabytes(2);
#else
  void *BaseAddress = (void *)(0);
#endif
  game_memory GameMemory = {};
  GameMemory.PersistentStorageSize = Megabytes(4);
  GameMemory.TransientStorageSize = Megabytes(8);//Gigabytes(1);

  uint64 TotalStorageSize = GameMemory.PersistentStorageSize + GameMemory.TransientStorageSize;
  GameMemory.PersistentStorage = mmap(BaseAddress, TotalStorageSize,
                                      PROT_READ | PROT_WRITE,
                                      MAP_ANON | MAP_PRIVATE,
                                      -1, 0);

  Assert(GameMemory.PersistentStorage!=nullptr,"nmap failed");
  GameMemory.TransientStorage = (uint8*)(GameMemory.PersistentStorage) + GameMemory.PersistentStorageSize;
  Assert(GameMemory.TransientStorage!=nullptr,"nmap failed");

  auto gameLib = load<game_library>("../plugin/libmusl_test_Game_plugin.so");

  gameLib.init = (GAME_INIT)dlsym(lib.pLibrary, "game_init");
  gameLib.deinit = (GAME_DEINIT)dlsym(lib.pLibrary, "game_deinit");
  gameLib.run = (GAME_RUN)dlsym(lib.pLibrary, "game_run");

  auto *gs = new (GameMemory.PersistentStorage) game_state;
  // auto *gs = (game_state*) GameMemory.PersistentStorage;

  gs->BlueOffset = 5;
  gs->GreenOffset = 2;
  gs->ToneHz = 3;

  if(gameLib.init(&GameMemory)) {
    gameLib.isInitialized = true;
  }

 gameLib.run(&GameMemory);
 gameLib.run(&GameMemory);
 gameLib.run(&GameMemory);
 gameLib.run(&GameMemory);
 gameLib.deinit(&GameMemory);

 DEBUGPlatformWriteEntireFile("gameMemory", GameMemory.PersistentStorageSize, GameMemory.PersistentStorage);
 munmap(GameMemory.PersistentStorage, GameMemory.PersistentStorageSize);

#if 1
 std::cout << "Reading entire memory file\n";
 auto mem_file = DEBUGPlatformReadEntireFile("gameMemory");
 std::cout << "Done\n";

 //M_Assert(mem_file.Contents != nullptr, "Assert");
 Assert(mem_file.Contents != nullptr, "");
#endif

#if 1
 GameMemory.PersistentStorageSize = mem_file.ContentsSize;
 GameMemory.PersistentStorage = mem_file.Contents;

 gs = (game_state*)GameMemory.PersistentStorage;
#elif 1
 gs = (game_state*)mem_file.Contents;
#else
 gs = (game_state*)GameMemory.PersistentStorage;
#endif

 Assert(gs->GreenOffset == 2,"");
 Assert(gs->ToneHz == 3,"");

 munmap(GameMemory.TransientStorage, GameMemory.TransientStorageSize);
 munmap(GameMemory.PersistentStorage, GameMemory.PersistentStorageSize);
 munmap(mem_file.Contents, mem_file.ContentsSize);



 return 0;
}

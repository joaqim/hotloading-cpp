#pragma once
#include <sys/mman.h> // nmap, unmap, mprotect

#include <CommonTypes.h>
#include <Log.h>

#ifdef MY_DEBUG
#define BaseAddress ((void *)Terabytes(2))
#else
#define BaseAddress ((void *)(0))
#endif

struct Memory {
  uint64 size;
  void *pStorage;
 Memory(uint64 size_, uint64 additionalSize_ = 0) :
  size(size_) {
    pStorage = mmap(BaseAddress, size_+additionalSize_,
                    PROT_READ | PROT_WRITE,
                    MAP_ANON | MAP_PRIVATE,
                    -1, 0);
    Assert(pStorage != nullptr,"nmap failed");
  }

 Memory(uint64 size_, Memory memory) :
  size(size_), pStorage{(uint8*)(memory.pStorage) + memory.size} {
    //pStorage = (uint8*)(memory.pStorage) + memory.size;
    Assert(pStorage == (uint8*)(memory.pStorage) + memory.size, "Memory(uint64 size_, Memory memory) failed in Memory.h");
  }
  ~Memory() {
    munmap(pStorage, size);
  }
};

struct GameState {
  int BlueOffset;
  int GreenOffset;
  int ToneHz;
};

struct GameMemory {
#if 0
 GameMemory(uint64 const persistentSize_, uint64 transientSize_) :
  persistent(persistentSize_, transientSize_), transient(transientSize_, persistent), isInitialized(false) {
  }

#endif

 GameMemory(uint64 persistentSize_, uint64 transientSize_ = 0) :
  persistentSize(persistentSize_), transientSize(transientSize_) {
    Assert(persistentSize_ > transientSize_, "PersistentStorageSize needs to be larger than TransientStorageSize");
    persistentStorage = mmap(
        BaseAddress, persistentSize_+transientSize_,
        PROT_READ | PROT_WRITE,
        MAP_ANON | MAP_PRIVATE,
        -1, 0);
    Assert(persistentStorage != nullptr,"nmap failed");
    transientStorage = (uint8*)(persistentStorage) + transientSize;
  }

  ~GameMemory() {
    munmap(transientStorage, transientSize);
    munmap(persistentStorage, persistentSize);
  }


  bool32 isInitialized = false;

  uint64 persistentSize;
  void *persistentStorage;
  uint64 transientSize;
  void *transientStorage;
};

typedef bool32(*GAME_INIT)(GameMemory*);
typedef void(*GAME_DEINIT)(GameMemory*);
typedef void(*GAME_RUN)(GameMemory*);


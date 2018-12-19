#pragma once
#include <xs/xs_Float.h>
#include <Log.h>
#include <iomanip>

typedef void(*PluginFunction)(void);
typedef void(*FUNC)(void);
typedef bool32(*FUNC_B)(void);

struct game_memory;

typedef bool32(*GAME_INIT)(game_memory*);
typedef void(*GAME_DEINIT)(game_memory*);
typedef void(*GAME_RUN)(game_memory*);


#define internal static
#define local_persist static
#define global_variable static

#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

inline uint32
SafeTruncateUInt64(uint64 Value) {
	Assert(Value <= 0xFFFFFFFF,"");
	uint32 Result = (uint32)Value;
	return(Result);
}

struct game_state {
  int BlueOffset;
  int GreenOffset;
  int ToneHz;
};

struct game_memory {
	uint64 PersistentStorageSize;
	void *PersistentStorage;
	uint64 TransientStorageSize;
	void *TransientStorage;

  bool32 isInitialized;
};


#pragma once
#include <unix_game.h>

struct File {
  uint32 size;
  void *pContents;
};

struct GameCode {
  GameCode() = default;
  void *pLibrary = nullptr;
  bool32 isValid = false;

  GAME_INIT Init(game_memory *gameMemory);
  GAME_DEINIT Deinit(game_memory *gameMemory);
  GAME_RUN Run(game_memory *gameMemory);
};

class GameLoader {
public:
  static File read(const char *filename);
  static void write(const char *filename, const void *memory, const uint32 size);
  static GameCode load(const char *filename);
  static void unload(GameCode &code);

private:
  GameCode _gameCode;
};

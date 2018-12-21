#include <iostream>

#include <CommonTypes.h>
#include <Memory.h>
#include <Log.h>

extern "C" {
  bool game_init(GameMemory *gameMemory) {
    TERRIFIC_INFO("game_init()");
    return true;
  }

  void game_deinit(GameMemory *gameMemory) {
    TERRIFIC_INFO("game_deinit()");
  }

  void game_run(GameMemory *gameMemory) {
    Assert(gameMemory->persistentSize > 0, "gameMemory empty");
    static int32 count{1};
    auto *state = (GameState *) gameMemory->persistentStorage;
    if(!gameMemory->isInitialized) {

      gameMemory->isInitialized = true;
      state->GreenOffset = 2;
      state->ToneHz = 3;
    }
    int i{0};
    state->BlueOffset += count++;
    TERRIFIC_INFO("Game run: {}", state->BlueOffset);

  }
}

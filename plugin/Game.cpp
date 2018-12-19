#include <unix_game.h>
#include <iostream>

extern "C" {
  bool game_init(game_memory *gameMemory) {
    return true;
  }

  void game_deinit(game_memory *gameMemory) {
  }

  void game_run(game_memory *gameMemory) {
    static int32 count{1};
    auto *state = (game_state *) gameMemory->PersistentStorage;

    if(!gameMemory->isInitialized) {
      gameMemory->isInitialized = true;
      //state->BlueOffset = 5;
      state->GreenOffset = 2;
      state->ToneHz = 3;
    }

    state->BlueOffset += count++;
    std::cout << "Game run: " << state->BlueOffset << "\n";
  }
}

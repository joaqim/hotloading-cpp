
#include <Log.h>
#include <FileWatcher.h>

#include <catch2/catch.hpp>

#include <chrono> // milliseconds
#include <thread> // this_thread::sleep_for
#include <iostream>
#include <curses.h>

TEST_CASE("FileWatcher") {
  //while(!Terrific::Utility::Log::isInitialized)
  //std::this_thread::sleep_for(std::chrono::milliseconds(60 * 15));

  //Terrific::Utility::Log::init();
  UpdateListener ul{};
  efsw::FileWatcher fileWatcher{};
  auto watchID = fileWatcher.addWatch("../plugin", &ul, false);
  REQUIRE(watchID > 0);

  //TERRIFIC_INFO("Watching directory {}, type any key to exit", "../plugin");
  printf("===========================================\n");
  printf("Watching directory %s, type any key to exit\n", "../plugin");
  return;
  char c{'\0'};
  while(c == '\0') {
    std::cin >> c;
    fileWatcher.watch();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }
  printf("===========================================\n");
  fileWatcher.removeWatch(watchID);
}

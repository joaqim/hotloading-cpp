#include <CommonTypes.h>
#include <Memory.h>
#include <CodeLoader.h>
#include <Log.h>

//#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#define STATIC_REQUIRE(Expression) {            \
    static_assert(Expression, #Expression);     \
    REQUIRE(Expression);                        \
  }

struct TestStruct  {
  TestStruct() = default;
  int x;
};

#define REQ_F(Expression) {                     \
    TERRIFIC_INFO(#Expression);                 \
    REQUIRE(Expression);                        \
  }

TEST_CASE("Creating GameMemory") {

  using Terrific::Utility::Log;

  Log::init();
  auto log = Log::getClientLogger();
  Log::toggleClient(0);
  //log->set_level(spdlog::level::off);

  GameMemory gameMemory{Megabytes(8), Megabytes(4)};

  REQUIRE(gameMemory.persistentSize == Megabytes(8));
  REQUIRE(gameMemory.transientSize == Megabytes(4));
  REQUIRE(gameMemory.persistentStorage != nullptr);
  REQUIRE(gameMemory.transientStorage != nullptr);

   auto *test = new (gameMemory.persistentStorage) TestStruct;
   test->x = 1;

   auto gameCode = CodeLoader::loadCode("../plugin/libmusl_test_Game_plugin.so");
   Assert(gameCode.isValid, "Error");

   REQ_F(!gameMemory.isInitialized);
   auto state = (GameState *)gameMemory.persistentStorage;
   REQ_F(state->BlueOffset == 1);
   REQ_F(gameCode.init(&gameMemory));
   LOG_F(gameCode.run(&gameMemory));
   REQ_F(gameMemory.isInitialized);
   REQ_F(state->BlueOffset > 1);

   LOG_F(gameCode.run(&gameMemory));
   LOG_F(gameCode.run(&gameMemory));
   LOG_F(gameCode.run(&gameMemory));
   LOG_F(gameCode.run(&gameMemory));
   LOG_F(gameCode.run(&gameMemory));

   LOG_F(gameCode.deinit(&gameMemory));

   LOG_F(CodeLoader::unloadCode(gameCode));
}

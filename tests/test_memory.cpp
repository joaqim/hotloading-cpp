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
}

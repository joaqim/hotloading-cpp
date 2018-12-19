/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file is part of the musl_test project:
 *     https://github.com/radupopescu/musl_test
 */

#include <catch2/catch.hpp>
#include <FakeIt/fakeit.hpp>

#include "dummy.h"

using namespace fakeit;

struct TestInterface {
	virtual int foo(int) = 0;
	virtual int bar(int, int) = 0;
};

TEST_CASE( "Dummy::Var() returns 0", "[dummy]" ) {
  musl_test::Dummy d;
  REQUIRE( d.Var() == 0);

  Mock<TestInterface> mock;
  When(Method(mock,foo)).Return(1);
  auto &interface = mock.get();

  REQUIRE(interface.foo(0) == 1);
}

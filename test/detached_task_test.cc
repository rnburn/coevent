#include "coevent/detached_task.h"

#include <experimental/coroutine>

#include "3rd_party/catch2/catch.hpp"

TEST_CASE(
    "Detached tasks support invoking coroutines that run detached from the "
    "point of invokation.") {

  SECTION("Detached task free their resources when completed.") {
    auto handle = [] () -> coevent::detached_task {
      co_return;
    }();
    REQUIRE(!handle.cancel());
  }

  SECTION("Suspended detached tasks can be canceld from their handle.") {
    auto handle = [] () -> coevent::detached_task {
      co_await std::experimental::suspend_always{};
    }();
    REQUIRE(handle.cancel());
  }
}

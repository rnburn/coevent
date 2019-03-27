#include "coevent/task.h"

#include <stdexcept>

#include "coevent/detached_task.h"
#include "3rd_party/catch2/catch.hpp"

static coevent::task<int> return_int(int x) {
  co_return x;
}

TEST_CASE("task is an awaitable that references the result of a coroutine.") {
  int value = 0;

  SECTION("We can access the value in a task after it's coroutine has completed.") {
    [&value]() -> coevent::detached_task {
      value = co_await return_int(123);
    }();
    REQUIRE(value == 123);
  }

  SECTION("We can co_await an l-value task.") {
    [&value]() -> coevent::detached_task {
      auto task = return_int(123);
      value = co_await task;
    }();
    REQUIRE(value == 123);
  }

  SECTION("If a task coroutine suspends, control is returned back to the caller.") {
    auto f = [] () -> coevent::task<int> {
      co_await std::experimental::suspend_always{};
      co_return 123;
    };
    auto detached_task = [&value, f]() -> coevent::detached_task {
      value = co_await f();
    }();
    REQUIRE(value == 0);
    REQUIRE(detached_task.cancel());
  }

  SECTION(
      "Exceptions thrown from a task coroutine are propagated up to the "
      "caller.") {
    auto f = []() -> coevent::task<int> {
      throw std::runtime_error{"abc"};
      co_return 123;
    };

    [&value, f]() -> coevent::detached_task {
      try {
        value = co_await f();
      } catch (const std::exception& /*e*/) {
          value = 456;
      }
    }();

    REQUIRE(value == 456);
  }

  SECTION("Void returning tasks are handled via a specialization.") {
    auto f = [] () -> coevent::task<void> {
      co_return;
    };
    [&value, f] () -> coevent::detached_task {
      co_await f();
      value = 123;
    }();
    REQUIRE(value == 123);
  }

  SECTION("Void returning tasks properly propagate exceptions.") {
    auto f = []() -> coevent::task<void> {
      throw std::runtime_error{"abc"};
      co_return;
    };

    [&value, f]() -> coevent::detached_task {
      try {
        co_await f();
        value = 123;
      } catch (const std::exception& /*e*/) {
          value = 456;
      }
    }();

    REQUIRE(value == 456);
  }
}

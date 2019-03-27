#include "coevent/sleep.h"

#include "3rd_party/catch2/catch.hpp"
#include "coevent/detached_task.h"

TEST_CASE(
    "Sleep returns an awaitable that resumes when the provided timeout "
    "elapses.") {
  coevent::io_context io_context;

  SECTION("Sleep suspends to the invoker.") {
    auto task = [&io_context]() -> coevent::detached_task {
      co_await coevent::sleep(io_context, std::chrono::milliseconds{100});
    }();
    REQUIRE(task.cancel());
  }

  SECTION("Sleep resumes when the timeout passes.") {
    auto task = [&io_context]() -> coevent::detached_task {
      co_await coevent::sleep(io_context, std::chrono::milliseconds{100});
    }();
    auto t1 = std::chrono::system_clock::now();
    io_context.run();
    auto t2 = std::chrono::system_clock::now();
    REQUIRE(!task.cancel());
    REQUIRE((t2 - t1) < std::chrono::milliseconds{150});
  }
}

#include "coevent/detached_task_promise.h"

#include <exception>

#include "src/detached_task_handle.h"

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
detached_task_promise::detached_task_promise()
    : handle_{std::make_shared<detached_task_handle>(
          std::experimental::coroutine_handle<
              detached_task_promise>::from_promise(*this))} {}

detached_task_promise::final_awaiter::final_awaiter(std::shared_ptr<detached_task_handle> handle) noexcept
  : handle_{std::move(handle)}
{}


//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
void detached_task_promise::final_awaiter::await_suspend(
        std::experimental::coroutine_handle<> /*coroutine*/) noexcept {
  handle_->finish();
}

//--------------------------------------------------------------------------------------------------
// get_return_object
//--------------------------------------------------------------------------------------------------
detached_task detached_task_promise::get_return_object() {
  return detached_task{handle_};
}

//--------------------------------------------------------------------------------------------------
// final_suspend
//--------------------------------------------------------------------------------------------------
detached_task_promise::final_awaiter detached_task_promise::final_suspend() noexcept {
  return final_awaiter{handle_};
}

//--------------------------------------------------------------------------------------------------
// unhandled_exception
//--------------------------------------------------------------------------------------------------
void detached_task_promise::unhandled_exception() const noexcept {
  std::terminate();
}
} // namespace coevent


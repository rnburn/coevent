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

//--------------------------------------------------------------------------------------------------
// get_return_object
//--------------------------------------------------------------------------------------------------
detached_task detached_task_promise::get_return_object() {
  return detached_task{handle_};
}

//--------------------------------------------------------------------------------------------------
// final_suspend
//--------------------------------------------------------------------------------------------------
std::experimental::suspend_never detached_task_promise::final_suspend() noexcept {
  handle_->finish();
  return {};
}

//--------------------------------------------------------------------------------------------------
// unhandled_exception
//--------------------------------------------------------------------------------------------------
void detached_task_promise::unhandled_exception() const noexcept {
  std::terminate();
}
} // namespace coevent


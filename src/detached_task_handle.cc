#include "src/detached_task_handle.h"

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
detached_task_handle::detached_task_handle(std::experimental::coroutine_handle<> coroutine) noexcept
  : coroutine_{coroutine}
{
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
detached_task_handle::~detached_task_handle() noexcept {
  finish();
}

//--------------------------------------------------------------------------------------------------
// finish
//--------------------------------------------------------------------------------------------------
void detached_task_handle::finish() noexcept {
  if(finished_.exchange(true)) {
    return;
  }
  coroutine_.destroy();
}
} // namespace coevent

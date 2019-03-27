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
// finish
//--------------------------------------------------------------------------------------------------
bool detached_task_handle::finish() noexcept {
  if(finished_.exchange(true)) {
    return false;
  }
  coroutine_.destroy();
  return true;
}
} // namespace coevent

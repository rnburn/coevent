#include "coevent/task.h"

namespace coevent {
//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
std::experimental::coroutine_handle<> task<void>::awaiter::await_suspend(
    std::experimental::coroutine_handle<> awaiting_coroutine) const noexcept {
  coroutine_.promise().set_continuation(awaiting_coroutine);
  return coroutine_;
}

//--------------------------------------------------------------------------------------------------
// await_resume
//--------------------------------------------------------------------------------------------------
void task<void>::awaiter::await_resume() const {
  assert(this->coroutine_ != nullptr);
  coroutine_.promise().result();
};
} // namespace coevent

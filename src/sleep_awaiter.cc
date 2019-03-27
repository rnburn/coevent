#include "coevent/sleep_awaiter.h"

namespace coevent {
//--------------------------------------------------------------------------------------------------
// sleep_awaiter
//--------------------------------------------------------------------------------------------------
sleep_awaiter::sleep_awaiter(coevent::io_context& io_context,
                             std::chrono::microseconds duration) noexcept
    : io_context_{io_context}, duration_{duration} {}

//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
void sleep_awaiter::await_suspend(
    std::experimental::coroutine_handle<> coroutine) {
  coroutine_ = coroutine;
  event_ = coevent::event{io_context_, -1, 0, &sleep_awaiter::on_timeout,
                          static_cast<void*>(this)};
  event_.activate(duration_);
}

//--------------------------------------------------------------------------------------------------
// on_timeout
//--------------------------------------------------------------------------------------------------
void sleep_awaiter::on_timeout(int /*file_descriptor*/, short /*what*/,
                               void* context) noexcept {
  auto& self = *static_cast<sleep_awaiter*>(context);
  self.coroutine_.resume();
}
}  // namespace coevent

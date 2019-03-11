#include "coevent/write_awaiter.h"

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
write_awaiter::write_awaiter(coevent::socket& socket,
                             std::span<const char> data,
                             size_t num_written) noexcept
    : socket_{socket}, data_{data}, num_written_{num_written} {
      (void)socket_;
}

//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
void write_awaiter::await_suspend(std::experimental::coroutine_handle<> coroutine) {
  coroutine_ = coroutine;
}
} // namespace coevent

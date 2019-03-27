#pragma once

#include "coevent/sleep_awaiter.h"

namespace coevent {
template <class Rep, class Period>
sleep_awaiter sleep(coevent::io_context& io_context, std::chrono::duration<Rep, Period> duration) {
  return {io_context, duration};
}
} // namespace coevent

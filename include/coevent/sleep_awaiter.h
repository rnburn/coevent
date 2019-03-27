#pragma once

#include <chrono>
#include <experimental/coroutine>

#include "coevent/event.h"
#include "coevent/io_context.h"

namespace coevent {
class sleep_awaiter {
 public:
   sleep_awaiter(coevent::io_context& io_context, std::chrono::microseconds duration) noexcept;

   template <class Rep, class Period>
   sleep_awaiter(coevent::io_context& io_context,
                 std::chrono::duration<Rep, Period> duration) noexcept
       : sleep_awaiter{
             io_context,
             std::chrono::duration_cast<std::chrono::microseconds>(duration)} {}

   void await_resume() const noexcept {}

   bool await_ready() const noexcept { return false; }

   void await_suspend(std::experimental::coroutine_handle<> coroutine);
 private:
  coevent::io_context& io_context_;
  std::chrono::microseconds duration_;
  coevent::event event_;
  std::experimental::coroutine_handle<> coroutine_{nullptr};
  std::exception_ptr exception_;

  static void on_timeout(int file_descriptor, short what, void* context) noexcept;
};
} // namespace coevent

#pragma once

#include <span>
#include <experimental/coroutine>
#include <exception>

#include "coevent/event.h"
#include "coevent/socket.h"

namespace coevent {
class write_awaiter {
 public:
   write_awaiter(coevent::socket& socket, std::span<const char> data, size_t num_written) noexcept;

   void await_resume() const;

   bool await_ready() const noexcept { return num_written_ == data_.size(); }

   void await_suspend(std::experimental::coroutine_handle<> coroutine);
 private:
  coevent::socket& socket_;
  coevent::event event_;
  std::span<const char> data_;
  size_t num_written_;
  std::experimental::coroutine_handle<> coroutine_{nullptr};
  std::exception_ptr exception_;

  static void on_writable(int file_descriptor, short what, void* context) noexcept;
};
} // namespace coevent

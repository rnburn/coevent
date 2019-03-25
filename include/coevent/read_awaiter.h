#pragma once

#include <span>
#include <experimental/coroutine>
#include <exception>

#include "coevent/event.h"
#include "coevent/socket.h"

namespace coevent {
class read_awaiter {
 public:
   read_awaiter(coevent::socket& socket, std::span<char> data, size_t num_read) noexcept;

   void await_resume() const;

   bool await_ready() const noexcept { return num_read_ == data_.size(); }

   void await_suspend(std::experimental::coroutine_handle<> coroutine);
 private:
  coevent::socket& socket_;
  coevent::event event_;
  std::span<char> data_;
  size_t num_read_;
  std::experimental::coroutine_handle<> coroutine_{nullptr};
  std::exception_ptr exception_;

  static void on_readable(int file_descriptor, short what, void* context) noexcept;
};
} // namespace coevent


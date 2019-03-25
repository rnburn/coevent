#pragma once

#include <experimental/coroutine>

#include "coevent/event.h"
#include "coevent/socket.h"

namespace coevent {
class [[nodiscard]] accept_awaiter {
 public:
  accept_awaiter(coevent::socket & listener_socket, coevent::socket&& accepted_socket) noexcept;

  coevent::socket await_resume();

  bool await_ready() const noexcept;

  void await_suspend(std::experimental::coroutine_handle<> coroutine);

 private:
  coevent::socket& listener_socket_;
  coevent::socket accepted_socket_;
  coevent::event event_;
  std::experimental::coroutine_handle<> coroutine_{nullptr};

  static void on_readable(int file_descriptor, short what, void* context) noexcept;
};
}  // namespace coevent

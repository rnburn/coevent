#pragma once

#include <experimental/coroutine>

#include "coevent/event.h"
#include "coevent/socket.h"

namespace coevent {
class [[nodiscard]] accept_awaiter {
 public:
  explicit accept_awaiter(coevent::socket & socket) noexcept;

  accept_awaiter(const accept_awaiter&) = delete;
  accept_awaiter(accept_awaiter&&) = delete;

  coevent::socket await_resume() const;

  bool await_ready() const noexcept { return false; }

  void await_suspend(std::experimental::coroutine_handle<> coroutine);

 private:
  coevent::socket& socket_;
  coevent::event event_;
  std::experimental::coroutine_handle<> coroutine_{nullptr};

  static void on_readable(int file_descriptor, short what, void* context) noexcept;
};

accept_awaiter accept(coevent::socket& socket);
}  // namespace coevent

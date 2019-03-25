#pragma once

#include "coevent/detached_task.h"

namespace coevent {

class detached_task_promise {
 public:
  class final_awaiter {
   public:
    explicit final_awaiter(
        std::shared_ptr<detached_task_handle> handle) noexcept;

    void await_resume() const noexcept {}

    bool await_ready() const noexcept { return false; }

    void await_suspend(
        std::experimental::coroutine_handle<> coroutine) noexcept;

   private:
    std::shared_ptr<detached_task_handle> handle_;
  };

  detached_task_promise();

  detached_task get_return_object();

  std::experimental::suspend_never initial_suspend() noexcept { return {}; }

  final_awaiter final_suspend() noexcept;

  void return_void() const noexcept {}

  void unhandled_exception() const noexcept;

 private:
  std::shared_ptr<detached_task_handle> handle_;
};
} // namespace coevent

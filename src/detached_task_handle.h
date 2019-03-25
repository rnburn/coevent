#pragma once

#include <atomic>
#include <experimental/coroutine>

namespace coevent {
class detached_task_handle {
 public:
   explicit detached_task_handle(std::experimental::coroutine_handle<> coroutine) noexcept;

   void finish() noexcept;
 private:
   std::atomic<bool> finished_;
   std::experimental::coroutine_handle<void> coroutine_;
};
} // namespace coevent

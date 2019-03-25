#pragma once

#include "coevent/detached_task.h"

namespace coevent {
class detached_task_promise {
 public:
   detached_task_promise();

   detached_task get_return_object();

   std::experimental::suspend_never initial_suspend() noexcept { return {}; }

   std::experimental::suspend_never final_suspend() noexcept;

   void return_void() const noexcept {}

   void unhandled_exception() const noexcept;

  private:
   std::shared_ptr<detached_task_handle> handle_;
};
} // namespace coevent

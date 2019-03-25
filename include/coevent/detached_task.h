#pragma once

#include <memory>
#include <experimental/coroutine>

namespace coevent {
class detached_task_promise;
class detached_task_handle;

class detached_task {
 public:
   using promise_type = detached_task_promise;

   explicit detached_task(std::shared_ptr<detached_task_handle> handle);

   void cancel() noexcept;
 private:
   std::shared_ptr<detached_task_handle> handle_;
};
} // namespace coevent

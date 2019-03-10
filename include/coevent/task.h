#pragma once

#include <experimental/coroutine>

namespace coevent {
template <class T>
class TaskPromise;

template <class T>
class Task {
 public:
   using promise_type = TaskPromise<T>;

   Task() noexcept = default;

   Task(std::experimental::coroutine_handle<> coroutine) noexcept
     : coroutine_{coroutine}
   {}

   std::experimental::coroutine_handle<> coroutine() const noexcept {
      return coroutine_;
   }
 private:
   std::experimental::coroutine_handle<> coroutine_{nullptr};
};

template <class T>
class TaskPromise {
 public:
   Task<T> get_return_object() {
     return Task<T>{std::experimental::coroutine_handle<TaskPromise>::from_promise(*this)};
   }

   auto initial_suspend() noexcept {
     return std::experimental::suspend_always{};
   }

   auto final_suspend() noexcept {
     return std::experimental::suspend_always{};
   }

   template <class U>
   void return_value(U&& /*u*/) {
   }

   void unhandled_exception() noexcept {
   }
};
} // namespace coevent

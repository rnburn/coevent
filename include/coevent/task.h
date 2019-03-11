#pragma once

#include <experimental/coroutine>

namespace coevent {
template <class T>
class task_promise;

template <class T>
class task {
 public:
   using promise_type = task_promise<T>;

   task() noexcept = default;

   task(std::experimental::coroutine_handle<> coroutine) noexcept
     : coroutine_{coroutine}
   {}

   std::experimental::coroutine_handle<> coroutine() const noexcept {
      return coroutine_;
   }
 private:
   std::experimental::coroutine_handle<> coroutine_{nullptr};
};

template <class T>
class task_promise {
 public:
   task<T> get_return_object() {
     return task<T>{std::experimental::coroutine_handle<task_promise>::from_promise(*this)};
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

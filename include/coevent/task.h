#pragma once

#include <experimental/coroutine>

namespace coevent {
template <class T>
class TaskPromise;

template <class T>
class Task {
 public:
   using promise_type = TaskPromise<T>;
 private:
};

template <class T>
class TaskPromise {
 public:
   Task<T> get_return_object() {
     return {};
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
};
} // namespace coevent

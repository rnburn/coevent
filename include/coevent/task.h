#pragma once

#include <experimental/coroutine>
#include <exception>
#include <variant>

namespace coevent {
template <class T>
class task_promise;

template <class T>
class task {
 public:
   using promise_type = task_promise<T>;

   class awaiter_base {
     public:
        explicit awaiter_base(std::experimental::coroutine_handle<promise_type> coroutine)
          : coroutine_{coroutine}
        {}

        // Awaiter
        bool await_ready() const noexcept {
          assert(coroutine_ != nullptr);
          return coroutine_.done();
        }

        std::experimental::coroutine_handle<> await_suspend(
            std::experimental::coroutine_handle<> awaiting_coroutine) const noexcept {
          coroutine_.promise().set_continuation(awaiting_coroutine);
          return coroutine_;
        }

     protected:
       std::experimental::coroutine_handle<promise_type> coroutine_;
   };

   class lvalue_awaiter final : public awaiter_base {
     public:
       using awaiter_base::awaiter_base;

       T& await_resume() const noexcept {
         assert(this->coroutine_ != nullptr);
         return this->coroutine_.promise().result();
       };
   };

   class rvalue_awaiter final : public awaiter_base {
     public:
       using awaiter_base::awaiter_base;

       T&& await_resume() const noexcept {
         assert(this->coroutine_ != nullptr);
         return std::move(this->coroutine_.promise().result());
       };
   };

   task() noexcept = default;

   task(std::experimental::coroutine_handle<promise_type> coroutine) noexcept
     : coroutine_{coroutine}
   {}

   task(const task& other) = delete;

   task(task&& other) noexcept : coroutine_{other.coroutine_} {
     other.coroutine_ = nullptr;
   }

   ~task() noexcept {
     if (coroutine_ != nullptr) {
       coroutine_.destroy();
     }
   }

   task& operator=(const task& other) = delete;

   task& operator=(task&& other) noexcept {
     assert(&other != this);
     if (coroutine_ != nullptr) {
       coroutine_.destroy();
     }
     coroutine_ = other.coroutine_;
     other.coroutine_ = nullptr;
   }

   lvalue_awaiter operator co_await() const& noexcept {
     return lvalue_awaiter{coroutine_};
   }

   rvalue_awaiter operator co_await() const&& noexcept {
     return rvalue_awaiter{coroutine_};
   }

 private:
   std::experimental::coroutine_handle<promise_type> coroutine_;
};

template <class T>
class task_promise {
 public:
   class final_awaiter {
    public:
     // Awaiter
     bool await_ready() const noexcept { return false; }

     void await_resume() const noexcept {}

     template <class Promise>
     std::experimental::coroutine_handle<> await_suspend(
         std::experimental::coroutine_handle<Promise> coroutine) noexcept {
       return coroutine.promise().continuation_;
     }
   };

   void set_continuation(std::experimental::coroutine_handle<> coroutine) noexcept {
     continuation_ = coroutine;
   }

   T& result() {
      if(auto result = std::get_if<0>(&result_); result != nullptr) {
        return *result;
      }
      std::rethrow_exception(std::get<1>(result_));
   }


   // Promise
   task<T> get_return_object() {
     return task<T>{std::experimental::coroutine_handle<task_promise>::from_promise(*this)};
   }

   std::experimental::suspend_always initial_suspend() noexcept {
     return {};
   }

   final_awaiter final_suspend() noexcept {
     return {};
   }

   template <class U, typename std::enable_if_t<std::is_convertible_v<U&&, T>, int> = 0 >
   void return_value(U&& u) noexcept(std::is_nothrow_constructible_v<T, U&&>) {
     result_.template emplace<0>(std::forward<U>(u));
   }

   void unhandled_exception() noexcept {
     result_.template emplace<1>(std::current_exception());
   }

  private:
   std::experimental::coroutine_handle<> continuation_;
   std::variant<T, std::exception_ptr> result_;
};
} // namespace coevent

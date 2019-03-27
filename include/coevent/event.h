#pragma once

#include <chrono>

#include "coevent/io_context.h"

namespace coevent {
class event {
 public:
  struct what {
    static const short write;
    static const short read;
  };

   using handle_type = void*;

   using callback_type = void (*)(int file_descriptor, short what, void* context) noexcept;

   event() noexcept = default;

   event(coevent::io_context& io_context, int file_descriptor, short what,
         callback_type callback, void* context);

   event(const event&) = delete;
   event(event&& other) noexcept;

   ~event() noexcept;

   event& operator=(const event&) = delete;
   event& operator=(event&& other) noexcept;

   void activate();

   void activate(std::chrono::microseconds timeout);

   template <class Rep, class Period>
   void activate(std::chrono::duration<Rep, Period> timeout) {
     return activate(std::chrono::duration_cast<std::chrono::microseconds>(timeout));
   }

   void deactivate();

  private:
   handle_type handle_{nullptr};

   void free() noexcept;
};
} // namespace coevent

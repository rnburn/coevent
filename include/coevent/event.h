#pragma once

#include "coevent/io_context.h"

namespace coevent {
class event {
 public:
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

   void deactivate();

  private:
   handle_type handle_{nullptr};

   void free() noexcept;
};
} // namespace coevent

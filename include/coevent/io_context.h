#pragma once

namespace coevent {
class io_context {
 public:
   using handle_type = void*;

   io_context();
   io_context(const io_context&) = delete;
   io_context(io_context&& other) noexcept;

   ~io_context() noexcept;

   io_context& operator=(const io_context&) = delete;
   io_context& operator=(io_context&& other) noexcept;

   void run();

   void stop() noexcept;

   handle_type handle() noexcept { return handle_; }
 private:
   handle_type handle_;

   void free() noexcept;
};
} // namespace coevent

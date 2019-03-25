#pragma once

#include "coevent/endpoint.h"
#include "coevent/io_context.h"

#include <cassert>

namespace coevent {
class socket {
 public:
   explicit socket(coevent::io_context& io_context) noexcept;
   socket(coevent::io_context& io_context, const coevent::endpoint& endpoint);
   socket(coevent::io_context& io_context, int file_descriptor) noexcept;
   socket(const socket&) = delete;
   socket(socket&& other) noexcept;

   ~socket() noexcept;

   socket& operator=(const socket&) = delete;
   socket& operator=(socket&& other) noexcept;

   io_context& io_context() noexcept {
    return io_context_;
   }

   int file_descriptor() noexcept { return file_descriptor_; }

   void connect(const coevent::endpoint& endpoint);

 private:
   coevent::io_context& io_context_;
   int file_descriptor_{-1};

   void free() noexcept;
};

void bind(coevent::socket& socket, const coevent::endpoint& endpoint);

void listen(coevent::socket& socket, int backlog);
} // namespace coevent

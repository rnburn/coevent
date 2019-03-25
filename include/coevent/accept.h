#pragma once

#include "coevent/socket.h"
#include "coevent/accept_awaiter.h"

namespace coevent {
class [[nodiscard]] accept_result {
 public:
   accept_result(coevent::socket& listener_socket, coevent::socket&& accepted_socket) noexcept;

   operator coevent::socket &&()&&;

   accept_awaiter operator co_await() noexcept;
  private:
   coevent::socket& listener_socket_;
   coevent::socket accepted_socket_;
};

accept_result accept(coevent::socket& socket);
} // namespace coevent

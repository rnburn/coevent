#include "coevent/accept.h"

#include <stdexcept>
#include <cerrno>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
accept_result::accept_result(coevent::socket& listener_socket,
                             coevent::socket&& accepted_socket) noexcept
    : listener_socket_{listener_socket},
      accepted_socket_{std::move(accepted_socket)} {}

//--------------------------------------------------------------------------------------------------
// operator coevent::socket&&
//--------------------------------------------------------------------------------------------------
accept_result::operator coevent::socket &&() && {
  if (accepted_socket_.file_descriptor() == -1) {
    throw std::runtime_error{"no socket accepted"};
  }
  return std::move(accepted_socket_);
}

//--------------------------------------------------------------------------------------------------
// operator co_await
//--------------------------------------------------------------------------------------------------
accept_awaiter accept_result::operator co_await() noexcept {
  return accept_awaiter{listener_socket_, std::move(accepted_socket_)};
}

//--------------------------------------------------------------------------------------------------
// accept
//--------------------------------------------------------------------------------------------------
accept_result accept(coevent::socket& listener_socket) {
  auto rcode = ::accept(listener_socket.file_descriptor(), nullptr, nullptr);
  if (rcode == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::ostringstream oss;
      oss << "write failed: " << std::strerror(errno);
      throw std::runtime_error{oss.str()};
    }
  }
  return accept_result{listener_socket,
                       coevent::socket{listener_socket.io_context(), rcode}};
}
} // namespace coevent


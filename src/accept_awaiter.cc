#include "coevent/accept_awaiter.h"

#include <cerrno>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include <sys/types.h>
#include <sys/socket.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
accept_awaiter::accept_awaiter(coevent::socket& socket) noexcept
    : socket_{socket} {}

//--------------------------------------------------------------------------------------------------
// await_resume
//--------------------------------------------------------------------------------------------------
coevent::socket accept_awaiter::await_resume() const {
  auto rcode = ::accept(socket_.file_descriptor(), nullptr, nullptr);
  if (rcode == -1) {
    std::ostringstream oss;
    oss << "accept failed: " << std::strerror(errno);
    throw std::runtime_error{oss.str()};
  }
  return {socket_.io_context(), rcode};
}

//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
void accept_awaiter::await_suspend(std::experimental::coroutine_handle<> coroutine) {
  coroutine_ = coroutine;
  event_ =
      coevent::event{socket_.io_context(), socket_.file_descriptor(),
                     coevent::event::what::read, &accept_awaiter::on_readable,
                     static_cast<void*>(this)};
  event_.activate();
}

//--------------------------------------------------------------------------------------------------
// on_readable
//--------------------------------------------------------------------------------------------------
void accept_awaiter::on_readable(int /*file_descriptor*/, short /*what*/, void* context) noexcept {
  auto& self = *static_cast<accept_awaiter*>(context);
  self.coroutine_.resume();
}

//--------------------------------------------------------------------------------------------------
// accept
//--------------------------------------------------------------------------------------------------
accept_awaiter accept(coevent::socket& socket) {
  return accept_awaiter{socket};
}
} // namespace coevent

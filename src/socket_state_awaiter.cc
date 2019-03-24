#include "coevent/socket_state_awaiter.h"

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
socket_state_awaiter::socket_state_awaiter(coevent::socket& socket, short what)
    : socket_{socket}, what_{what} {}

//--------------------------------------------------------------------------------------------------
// await_resume
//--------------------------------------------------------------------------------------------------
short socket_state_awaiter::await_resume() const noexcept {
  return state_what_;
}

//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
void socket_state_awaiter::await_suspend(std::experimental::coroutine_handle<> coroutine) {
  coroutine_ = coroutine;
  event_ = coevent::event{socket_.io_context(), socket_.file_descriptor(),
                          what_, &socket_state_awaiter::on_state_change,
                          static_cast<void*>(this)};
  event_.activate();
}

//--------------------------------------------------------------------------------------------------
// on_state_change
//--------------------------------------------------------------------------------------------------
void socket_state_awaiter::on_state_change(int /*file_descriptor*/, short what,
                                           void* context) noexcept {
  auto& self = *static_cast<socket_state_awaiter*>(context);
  self.state_what_ = what;
  self.coroutine_.resume();
}

//--------------------------------------------------------------------------------------------------
// until_readable
//--------------------------------------------------------------------------------------------------
socket_state_awaiter until_readable(coevent::socket& socket) {
  return {socket, event::what::read};
}

//--------------------------------------------------------------------------------------------------
// until_writable
//--------------------------------------------------------------------------------------------------
socket_state_awaiter until_writable(coevent::socket& socket) {
  return {socket, event::what::write};
}
} // namespace coevent

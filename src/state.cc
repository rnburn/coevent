#include "coevent/state.h"

namespace coevent {
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

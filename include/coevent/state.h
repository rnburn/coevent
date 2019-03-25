#pragma once

#include "coevent/socket_state_awaiter.h"

namespace coevent {
socket_state_awaiter until_readable(coevent::socket& socket);

socket_state_awaiter until_writable(coevent::socket& socket);
} // namespace coevent

#include "coevent/write.h"

#include <stdexcept>
#include <cerrno>
#include <sstream>

#include <unistd.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
write_result::write_result(coevent::socket& socket, std::span<const char> data,
                           size_t num_written) noexcept
    : socket_{socket}, data_{data}, num_written_{num_written} {
      (void)socket_;
      (void)data_;
    }

//--------------------------------------------------------------------------------------------------
// write
//--------------------------------------------------------------------------------------------------
coevent::write_result write(coevent::socket& socket,
                            std::span<const char> data) {
  auto rcode = ::write(socket.file_descriptor(),
                       static_cast<const void*>(data.data()), data.size());
  if (rcode == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::ostringstream oss;
      oss << "write failed: " << std::strerror(errno);
      throw std::runtime_error{oss.str()};
    }
    rcode = 0;
  }
  return write_result{socket, data, static_cast<size_t>(rcode)};
}
}  // namespace coevent

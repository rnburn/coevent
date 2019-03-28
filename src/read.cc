#include "coevent/read.h"

#include <stdexcept>
#include <cerrno>
#include <sstream>

#include <unistd.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
read_result::read_result(coevent::socket& socket, std::span<char> data,
                           size_t num_read, bool eof) noexcept
    : socket_{socket}, data_{data}, num_read_{num_read}, eof_{eof} {}

//--------------------------------------------------------------------------------------------------
// operator co_await
//--------------------------------------------------------------------------------------------------
read_awaiter read_result::operator co_await() const {
  if (eof_) {
    throw std::runtime_error{"unexpected eof"};
  }
  return {socket_, data_, num_read_};
}

//--------------------------------------------------------------------------------------------------
// read
//--------------------------------------------------------------------------------------------------
coevent::read_result read(coevent::socket& socket,
                            std::span<char> data) {
  auto rcode = ::read(socket.file_descriptor(),
                       static_cast<void*>(data.data()), data.size());
  bool eof = false;
  if (rcode == -1) {
    if (errno != EAGAIN && errno != EWOULDBLOCK) {
      std::ostringstream oss;
      oss << "read failed: " << std::strerror(errno);
      throw std::runtime_error{oss.str()};
    }
    rcode = 0;
  } else if (rcode == 0) {
    eof = true;
  }
  return read_result{socket, data, static_cast<size_t>(rcode), eof};
}
}  // namespace coevent


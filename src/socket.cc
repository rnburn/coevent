#include "coevent/socket.h"

#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <sstream>

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// set_reuse_address
//--------------------------------------------------------------------------------------------------
static void set_reuse_address(int file_descriptor) {
  int optvalue = 1;
  int rcode = ::setsockopt(file_descriptor, SOL_SOCKET, SO_REUSEADDR,
                           static_cast<void*>(&optvalue), sizeof(int));
  if (rcode == -1) {
    std::ostringstream oss;
    oss << "failed to set the socket as reusable: : " << std::strerror(errno);
    throw std::runtime_error{oss.str()};
  }
}

//--------------------------------------------------------------------------------------------------
// set_nonblocking
//--------------------------------------------------------------------------------------------------
static void set_nonblocking(int file_descriptor) {
  int rcode = ::fcntl(file_descriptor, F_SETFL,
                      ::fcntl(file_descriptor, F_GETFL, 0) | O_NONBLOCK);
  if (rcode == -1) {
    std::ostringstream oss;
    oss << "failed to set the socket as non-blocking: " << std::strerror(errno);
    throw std::runtime_error{oss.str()};
  }
}

//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
socket::socket(coevent::io_context& io_context) noexcept
  : io_context_{io_context}
{}

socket::socket(coevent::io_context& io_context, const coevent::endpoint& endpoint) 
  : io_context_{io_context}
{
  this->connect(endpoint);
}

socket::socket(coevent::io_context& io_context, int file_descriptor) noexcept
  : io_context_{io_context}, file_descriptor_{file_descriptor}
{}

socket::socket(socket&& other) noexcept 
  : io_context_{other.io_context_},
    file_descriptor_{other.file_descriptor_}
{
  other.file_descriptor_ = -1;
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
socket::~socket() noexcept {
  free();
}

//--------------------------------------------------------------------------------------------------
// operator=
//--------------------------------------------------------------------------------------------------
socket& socket::operator=(socket&& other) noexcept {
  assert(&other != this);
  free();
  file_descriptor_ = other.file_descriptor_;
  other.file_descriptor_ = -1;
  return *this;
}

//--------------------------------------------------------------------------------------------------
// connect
//--------------------------------------------------------------------------------------------------
void socket::connect(const coevent::endpoint& endpoint) {
  this->open(endpoint.family());
  auto rcode =
      ::connect(file_descriptor_, &static_cast<const sockaddr&>(endpoint),
                endpoint.length());
  if (rcode == -1 && errno != EINPROGRESS) {
    std::ostringstream oss;
    oss << "connect failed: " << std::strerror(errno);
    throw std::runtime_error{oss.str()};
  }
}

//--------------------------------------------------------------------------------------------------
// bind
//--------------------------------------------------------------------------------------------------
void socket::bind(const coevent::endpoint& endpoint) {
  this->open(endpoint.family());
  auto rcode = ::bind(file_descriptor_, &static_cast<const sockaddr&>(endpoint),
                      endpoint.length());
  if (rcode == -1) {
    std::ostringstream oss;
    oss << "bind failed: " << std::strerror(errno);
    throw std::runtime_error{oss.str()};
  }
}

//--------------------------------------------------------------------------------------------------
// construct
//--------------------------------------------------------------------------------------------------
void socket::open(int family) {
  file_descriptor_ = ::socket(family, SOCK_STREAM, 0);
  if (file_descriptor_ == -1) {
    std::ostringstream oss;
    oss << "socket failed: " << std::strerror(errno);
    throw std::runtime_error{oss.str()};
  }
  set_nonblocking(file_descriptor_);
  set_reuse_address(file_descriptor_);
}

//--------------------------------------------------------------------------------------------------
// free
//--------------------------------------------------------------------------------------------------
void socket::free() noexcept {
  if (file_descriptor_ != -1) {
    ::close(file_descriptor_);
  }
}

//--------------------------------------------------------------------------------------------------
// listen
//--------------------------------------------------------------------------------------------------
void listen(coevent::socket& socket, int backlog) {
  auto rcode = ::listen(socket.file_descriptor(), backlog);
  if (rcode == -1) {
    std::ostringstream oss;
    oss << "listen failed: " << std::strerror(errno);
    throw std::runtime_error{oss.str()};
  }
}
} // namespace coevent

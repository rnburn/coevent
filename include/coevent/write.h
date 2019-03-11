#pragma once

#include <cstddef>

#include "coevent/socket.h"

namespace coevent {
class write_result {
 public:
   operator size_t const noexcept { return num_written_; }
 private:
  // coevent::socket& socket_;
  // DataRef data_;
  size_t num_written_;
};

// coevent::write_result write(coevent::socket& socket, /*data reference*/);
} // namespace coevent

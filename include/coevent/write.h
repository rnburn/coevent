#pragma once

#include <cstddef>
#include <span>

#include "coevent/socket.h"
#include "coevent/write_awaiter.h"

namespace coevent {
class [[nodiscard]] write_result {
 public:
   write_result(coevent::socket& socket, std::span<const char> data, size_t num_written) noexcept;

   operator size_t() const noexcept { return num_written_; }

   write_awaiter operator co_await() const noexcept;
 private:
  coevent::socket& socket_;
  std::span<const char> data_;
  size_t num_written_;
};

coevent::write_result write(coevent::socket& socket, std::span<const char> data);
} // namespace coevent

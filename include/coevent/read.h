#pragma once

#include <cstddef>
#include <span>

#include "coevent/socket.h"
#include "coevent/read_awaiter.h"

namespace coevent {
class [[nodiscard]] read_result {
 public:
   read_result(coevent::socket& socket, std::span<char> data, size_t num_read, bool eof) noexcept;

   operator size_t() const noexcept { return num_read_; }

   size_t num_read() const noexcept { return num_read_; }

   bool eof() const noexcept { return eof_; }

   read_awaiter operator co_await() const;
 private:
   coevent::socket& socket_;
   std::span<char> data_; 
   size_t num_read_;
   bool eof_;
};

read_result read(coevent::socket& socket, std::span<char> data);
} // namesapce coevent

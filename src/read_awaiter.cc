#include "coevent/read_awaiter.h"

#include <cerrno>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include <unistd.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
read_awaiter::read_awaiter(coevent::socket& socket,
                             std::span<char> data,
                             size_t num_read) noexcept
    : socket_{socket}, data_{data}, num_read_{num_read} {
}

//--------------------------------------------------------------------------------------------------
// await_resume
//--------------------------------------------------------------------------------------------------
void read_awaiter::await_resume() const {
  if (exception_ != nullptr) {
    std::rethrow_exception(exception_);
  }
}

//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
void read_awaiter::await_suspend(std::experimental::coroutine_handle<> coroutine) {
  coroutine_ = coroutine;
  event_ =
      coevent::event{socket_.io_context(), socket_.file_descriptor(),
                     coevent::event::what::read, &read_awaiter::on_readable,
                     static_cast<void*>(this)};
  event_.activate();
}

//--------------------------------------------------------------------------------------------------
// on_readable
//--------------------------------------------------------------------------------------------------
void read_awaiter::on_readable(int /*file_descriptor*/, short /*what*/,
                                void* context) noexcept {
  auto& self = *static_cast<read_awaiter*>(context);
  try {
    auto rcode =
        ::read(self.socket_.file_descriptor(),
                static_cast<void*>(self.data_.data() + self.num_read_),
                self.data_.size() - self.num_read_);
    if (rcode == -1) {
      std::ostringstream oss;
      oss << "read failed: " << std::strerror(errno);
      throw std::runtime_error{oss.str()};
    }
    if (rcode == 0) {
      throw std::runtime_error{"unexpected eof"};
    }
    self.num_read_ += static_cast<size_t>(rcode);
    if (self.num_read_ == self.data_.size()) {
      self.coroutine_.resume();
    }
  } catch (...) {
    self.exception_ = std::current_exception();
    self.coroutine_.resume();
  }
}
} // namespace coevent


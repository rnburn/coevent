#include "coevent/write_awaiter.h"

#include <cerrno>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include <unistd.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
write_awaiter::write_awaiter(coevent::socket& socket,
                             std::span<const char> data,
                             size_t num_written) noexcept
    : socket_{socket}, data_{data}, num_written_{num_written} {
}

//--------------------------------------------------------------------------------------------------
// await_resume
//--------------------------------------------------------------------------------------------------
void write_awaiter::await_resume() const {
  if (exception_ != nullptr) {
    std::rethrow_exception(exception_);
  }
}

//--------------------------------------------------------------------------------------------------
// await_suspend
//--------------------------------------------------------------------------------------------------
void write_awaiter::await_suspend(std::experimental::coroutine_handle<> coroutine) {
  coroutine_ = coroutine;
  event_ =
      coevent::event{socket_.io_context(), socket_.file_descriptor(),
                     coevent::event::what::write, &write_awaiter::on_writable,
                     static_cast<void*>(this)};
  event_.activate();
}

//--------------------------------------------------------------------------------------------------
// on_writable
//--------------------------------------------------------------------------------------------------
void write_awaiter::on_writable(int /*file_descriptor*/, short /*what*/,
                                void* context) noexcept {
  auto& self = *static_cast<write_awaiter*>(context);
  try {
    auto rcode =
        ::write(self.socket_.file_descriptor(),
                static_cast<const void*>(self.data_.data() + self.num_written_),
                self.data_.size() - self.num_written_);
    if (rcode == -1) {
      std::ostringstream oss;
      oss << "write failed: " << std::strerror(errno);
      throw std::runtime_error{oss.str()};
    }
    self.num_written_ += static_cast<size_t>(rcode);
    if (self.num_written_ == self.data_.size()) {
      self.coroutine_.resume();
    }
  } catch (...) {
    self.exception_ = std::current_exception();
    self.coroutine_.resume();
  }
}
} // namespace coevent

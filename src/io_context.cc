#include "coevent/io_context.h"

#include <exception>
#include <stdexcept>
#include <cassert>
#include <iostream>

#include <event2/event.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
io_context::io_context() {
  handle_ = static_cast<void*>(event_base_new());
  if (handle_ == nullptr) {
    throw std::runtime_error{"event_base_new failed"};
  }
}

io_context::io_context(io_context&& other) noexcept {
  handle_ = other.handle_;
  other.handle_ = nullptr;
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
io_context::~io_context() noexcept {
  free();
}

//--------------------------------------------------------------------------------------------------
// operator=
//--------------------------------------------------------------------------------------------------
io_context& io_context::operator=(io_context&& other) noexcept {
  if (&other == this) {
    return *this;
  }
  free();
  handle_ = other.handle_;
  other.handle_ = nullptr;
  return *this;
}

//--------------------------------------------------------------------------------------------------
// run
//--------------------------------------------------------------------------------------------------
void io_context::run() {
  assert(handle_ != nullptr);
  auto rcode = event_base_dispatch(static_cast<event_base*>(handle_));
  if (rcode == -1) {
    throw std::runtime_error{"event_base_dispatch failed"};
  }
}

//--------------------------------------------------------------------------------------------------
// stop
//--------------------------------------------------------------------------------------------------
void io_context::stop() noexcept {
  assert(handle_ != nullptr);
  auto rcode = event_base_loopbreak(static_cast<event_base*>(handle_));
  if (rcode != 0) {
    std::cerr << "event_base_loopbreak failed\n";
    std::terminate();
  }
}

//--------------------------------------------------------------------------------------------------
// free
//--------------------------------------------------------------------------------------------------
void io_context::free() noexcept {
  if (handle_ != nullptr) {
    event_base_free(static_cast<event_base*>(handle_));
    handle_ = nullptr;
  }
}
} // namespace coevent

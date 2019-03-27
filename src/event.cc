#include "coevent/event.h"

#include <cassert>
#include <stdexcept>

#include <event2/event.h>

#include <sys/time.h>

namespace coevent {
//--------------------------------------------------------------------------------------------------
// to_timeval
//--------------------------------------------------------------------------------------------------
static timeval to_timeval(std::chrono::microseconds microseconds) {
  timeval result;
  auto num_microseconds = microseconds.count();
  const size_t microseconds_in_second = 1000000;
  result.tv_sec =
      static_cast<time_t>(num_microseconds / microseconds_in_second);
  result.tv_usec =
      static_cast<suseconds_t>(num_microseconds % microseconds_in_second);
  return result;
}

//--------------------------------------------------------------------------------------------------
// what
//--------------------------------------------------------------------------------------------------
const short event::what::write = EV_WRITE;
const short event::what::read = EV_READ;

//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
event::event(coevent::io_context& io_context, int file_descriptor, short what,
             callback_type callback, void* context) {
  handle_ = static_cast<void*>(
      event_new(static_cast<event_base*>(io_context.handle()), file_descriptor,
                what, callback, context));
  if (handle_ == nullptr) {
    throw std::runtime_error{"event_new failed"};
  }
}

event::event(event&& other) noexcept 
  : handle_{other.handle_}
{
  other.handle_ = nullptr;
}

//--------------------------------------------------------------------------------------------------
// destructor
//--------------------------------------------------------------------------------------------------
event::~event() noexcept {
  free();
}

//--------------------------------------------------------------------------------------------------
// operator=
//--------------------------------------------------------------------------------------------------
event& event::operator=(event&& other) noexcept {
  assert(&other != this);
  free();
  handle_ = other.handle_;
  other.handle_ = nullptr;
  return *this;
}

//--------------------------------------------------------------------------------------------------
// activate
//--------------------------------------------------------------------------------------------------
void event::activate() {
  auto rcode = event_add(static_cast<::event*>(handle_), nullptr);
  if (rcode != 0) {
    throw std::runtime_error{"event_add failed"};
  }
}

void event::activate(std::chrono::microseconds timeout) {
  auto tv = to_timeval(timeout);
  auto rcode = event_add(static_cast<::event*>(handle_), &tv);
  if (rcode != 0) {
    throw std::runtime_error{"event_add failed"};
  }
}

//--------------------------------------------------------------------------------------------------
// deactivate
//--------------------------------------------------------------------------------------------------
void event::deactivate() {
  auto rcode = event_del(static_cast<::event*>(handle_));
  if (rcode != 0) {
    throw std::runtime_error{"event_del failed"};
  }
}

//--------------------------------------------------------------------------------------------------
// free
//--------------------------------------------------------------------------------------------------
void event::free() noexcept {
  if (handle_ != nullptr) {
    event_free(static_cast<::event*>(handle_));
  }
}
}  // namespace coevent

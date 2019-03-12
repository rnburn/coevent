#include "coevent/event.h"

#include <stdexcept>

#include <event2/event.h>

namespace coevent {
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

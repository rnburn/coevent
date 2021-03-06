#include "coevent/detached_task.h"

#include "src/detached_task_handle.h"

namespace coevent {
//--------------------------------------------------------------------------------------------------
// constructor
//--------------------------------------------------------------------------------------------------
detached_task::detached_task(std::shared_ptr<detached_task_handle> handle) 
  : handle_{std::move(handle)}
{}

//--------------------------------------------------------------------------------------------------
// cancel
//--------------------------------------------------------------------------------------------------
bool detached_task::cancel() noexcept {
  return handle_->finish();
}
} // namespace coevent

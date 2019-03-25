#include <iostream>
#include <cstring>

#include "coevent/write.h"
#include "coevent/detached_task.h"
#include "coevent/detached_task_promise.h"

coevent::detached_task f(coevent::socket& socket) {
  const char* msg = "abc123";
  co_await coevent::write(socket, {msg, std::strlen(msg)});
}

int main() {
  std::cout << "nuf\n";
  return 0;
}

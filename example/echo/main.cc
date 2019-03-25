#include <iostream>
#include <cstring>

#include "coevent/write.h"
#include "coevent/accept_awaiter.h"
#include "coevent/detached_task.h"
#include "coevent/detached_task_promise.h"

coevent::detached_task run_server(coevent::socket& socket) {
  auto new_socket = co_await coevent::accept(socket);
  (void)new_socket;
  std::cout << "new connection" << std::endl;
  /* const char* msg = "abc123"; */
  /* co_await coevent::write(socket, {msg, std::strlen(msg)}); */
}

int main() {
  coevent::io_context io_context;
  coevent::endpoint endpoint{"127.0.0.1", 8080};
  coevent::socket socket{io_context};
  socket.bind(endpoint);
  listen(socket, 10);
  run_server(socket);
  io_context.run();
  std::cout << "nuf\n";
  return 0;
}

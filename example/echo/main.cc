#include <iostream>
#include <array>
#include <cstring>

#include "coevent/write.h"
#include "coevent/accept_awaiter.h"
#include "coevent/detached_task.h"
#include "coevent/detached_task_promise.h"

/* coevent::detached_task process_session(coevent::socket socket) { */
/*   std::array<char, 500> buffer; */
/*   while (true) { */
/*     co_await coevent::until_readable(socket); */
/*   } */
/* } */

coevent::detached_task run_server(coevent::socket& socket) {
  co_await std::experimental::suspend_never{};
  coevent::accept_awaiter awaiter{socket};
  auto new_socket = co_await awaiter;
  (void)new_socket;
  std::cout << "new connection" << std::endl;
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

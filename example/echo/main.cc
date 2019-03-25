#include <iostream>
#include <array>
#include <cstring>

#include "coevent/write.h"
#include "coevent/read.h"
#include "coevent/accept.h"
#include "coevent/state.h"
#include "coevent/detached_task.h"

coevent::detached_task process_session(coevent::socket socket) {
  std::array<char, 500> buffer;
  while (true) {
    co_await coevent::until_readable(socket);
    auto read_result = coevent::read(socket, {buffer.data(), buffer.size()});
    if (read_result.eof()) {
      co_return;
    }
    co_await coevent::write(socket, {buffer.data(), read_result.num_read()});
  }
}

coevent::detached_task run_server(coevent::socket& listener_socket) {
  while (true) {
    auto socket = co_await accept(listener_socket);
    process_session(std::move(socket));
  }
}

int main() {
  coevent::io_context io_context;
  coevent::endpoint endpoint{"127.0.0.1", 8080};
  coevent::socket socket{io_context};
  socket.bind(endpoint);
  listen(socket, 10);
  run_server(socket);
  io_context.run();
  return 0;
}

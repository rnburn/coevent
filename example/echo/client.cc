#include <iostream>
#include <array>
#include <cstring>
#include <string>
#include <random>

#include "coevent/write.h"
#include "coevent/read.h"
#include "coevent/accept.h"
#include "coevent/sleep.h"
#include "coevent/task.h"
#include "coevent/detached_task.h"

static coevent::task<void> random_sleep(coevent::io_context& io_context) {
  static thread_local std::mt19937 random_number_generator{std::random_device{}()};
  std::uniform_int_distribution distribution(0, 1000);
  co_await coevent::sleep(
      io_context,
      std::chrono::milliseconds{distribution(random_number_generator)});
}

static coevent::detached_task run_client(coevent::io_context& io_context, int index) {
  co_await random_sleep(io_context);

  coevent::socket socket{io_context, coevent::endpoint{"127.0.0.1", 8080}};

  auto send_message = "Hello from " + std::to_string(index);
  std::cout << "Sent: " << send_message << "\n";
  co_await coevent::write(socket, {send_message.data(), send_message.size()});

  std::string received_message(send_message.size(), ' ');
  co_await coevent::read(socket, {received_message.data(), received_message.size()});
  std::cout << "Received: " << received_message << "\n";
}

int main() {
  coevent::io_context io_context;
  for (int i=0; i<10; ++i) {
    run_client(io_context, i);
  }
  io_context.run();
  return 0;
}

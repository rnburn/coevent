# coevent

A C++ coroutine library for asynchronous networking. (Based off of libevent).

## Quick Start

Make a simple echo server and client

```cpp
// server

static coevent::detached_task process_session(coevent::socket socket) {
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

static coevent::detached_task run_server(coevent::socket& listener_socket) {
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

```

```cpp
// client

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
```

See [example/echo](example/echo) for full example.

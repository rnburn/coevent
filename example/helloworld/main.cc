#include <iostream>

#include "coevent/task.h"

coevent::Task<int> f() {
  std::cout << "arf" << std::endl;
  co_return 22;
}

int main() {
  std::cout << "nuf\n";
  auto task = f();
  task.coroutine().resume();
  return 0;
}

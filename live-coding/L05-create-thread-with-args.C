#include <thread>
#include <iostream>

void run(int i) {
  std::cout << "In run " << i << "\n";
}

int main() {
  for (int i = 0; i < 10; ++i) {
    std::thread t1(run, i);
    t1.join();
  }
}

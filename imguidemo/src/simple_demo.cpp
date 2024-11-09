#include <iostream>

#include "simple_demo_gui.h"

int main(int argc, char **argv) {
#if 0
  std::thread thread(thread_gui);
  thread.join();
#endif

  thread_gui();

  return 0;
}

#include <iostream>
#include <exception>

#include "threadsafequeue.h"

int main(int argc, char** argv) try {
    threadsafe::ThreadSafeQueue<int> queue;
    queue.push(1);
    queue.push(2);

    auto res_value = 0;
    queue.wait_and_pop(res_value);
    std::cout << res_value << " ";
    std::cout << std::endl;

    return 0;
}
catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
}

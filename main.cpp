#include <iostream>
#include <exception>

int main(int argc, char** argv) try {


    return 0;
}
catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return -1;
}

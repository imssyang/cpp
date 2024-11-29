
#include <iostream>
#include <thread>
#include <exception>

std::exception_ptr eptr; // 用于存储异常

void worker() {
    try {
        throw std::runtime_error("Error in worker thread");
    } catch (...) {
        eptr = std::current_exception(); // 捕获异常
    }
}

int main() {
    std::thread t(worker);
    t.join();

    if (eptr) {
        try {
            std::rethrow_exception(eptr); // 重新抛出异常
        } catch (const std::exception& e) {
            std::cout << "Caught exception: " << e.what() << std::endl;
        }
    }

    return 0;
}


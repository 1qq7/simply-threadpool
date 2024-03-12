
#include <iostream>
#include <random>

#include "threadpool/threadpool.h"

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);

    simply::ThreadPool pool(4, 6);

    for (int i = 0; i < 100; ++i) {
        pool.enqueue([&dis, &gen] (int i){
            int random_number = dis(gen);

            std::cout << "Task " << i << " executed, " << random_number
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(
                random_number));  // Simulating a long task
        }, i);
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    return 0;
}

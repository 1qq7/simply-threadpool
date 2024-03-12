#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace simply {

class ThreadPool {
   public:
    ThreadPool(size_t numThreads, size_t maxQueueSize)
        : stop(false), maxQueueSize(maxQueueSize), queueSize(0) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->cv_runtask.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                        });
                        if (this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        --this->queueSize;
                    }

                    task();

                    this->cv_enqueue.notify_one();
                }
            });
        }
    }

    template <class F, class... Args>
    void enqueue(F&& f, Args&&... args) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv_enqueue.wait(lock, [this] {
                return !this->stop && this->queueSize < this->maxQueueSize;
            });
            if (this->stop) return;
            auto task =
                std::bind(std::forward<F>(f), std::forward<Args>(args)...);
            tasks.emplace([task]() { task(); });
            ++queueSize;
        }
        cv_runtask.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        cv_runtask.notify_all();
        cv_enqueue.notify_all();
        for (std::thread& worker : workers) worker.join();
    }

   private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queueMutex;
    std::condition_variable cv_runtask;
    std::condition_variable cv_enqueue;

    bool stop;
    size_t maxQueueSize;
    std::atomic<size_t> queueSize;
};

}  // namespace simply

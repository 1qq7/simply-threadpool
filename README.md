# SimplyThreadPool

SimplyThreadPool 是一个简单的 C++ 线程池实现，它可以帮助你在多线程环境下管理任务并提高程序的并发性。

## 特点

- **简单易用**：简单的 API 设计，易于集成到你的项目中。
- **基于 C++11**：使用 C++11 标准实现，兼容现代 C++ 程序。
- **灵活配置**：可以配置线程池的线程数量和最大任务队列大小。
- **轻量级**：轻量级的实现，HEAD-ONLY，无需引入其他复杂的依赖。

## 使用示例

```cpp
#include <iostream>
#include "threadpool/threadpool.h"

int main() {
    simply::ThreadPool pool(4, 100); // 创建一个最多同时运行 4 个线程，任务队列最大为 100 的线程池

    // 向线程池添加任务
    for (int i = 0; i < 10; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " executed by thread " << std::this_thread::get_id() << std::endl;
        });
    }

    // 等待所有任务执行完成
    // 注意：ThreadPool 对象在销毁时会自动等待所有任务执行完成
    return 0;
}
```

## 构建

你可以使用 CMake 来构建该项目：

```bash
cd sample
mkdir build
cd build
cmake ..
make
```

## API 文档

### `ThreadPool(size_t numThreads, size_t maxQueueSize)`

构造函数，创建一个线程池对象。

- `numThreads`：指定线程池中的线程数量。
- `maxQueueSize`：指定任务队列的最大容量。

### `template <class F, class... Args> void enqueue(F&& f, Args&&... args)`

将任务添加到线程池的任务队列中。

- `f`：要执行的任务函数。
- `args`：任务函数的参数。

### `~ThreadPool()`

析构函数，销毁线程池对象。

## 贡献

欢迎贡献代码或提出建议！如果你发现了任何问题或有改进建议，请提交 issue 或者直接提起一个 pull request。

## 许可证

该项目采用 MIT 许可证进行许可 - [LICENSE](LICENSE)

#include "ThreadPoolManager.h"


// 생성자
ThreadPoolManager::ThreadPoolManager(size_t numThreads) 
    : mbStop(false) 
{
    for (size_t i = 0; i < numThreads; ++i) {
        mWorkers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(mTasksMutex);
                    mCondition.wait(lock, [this] { return mbStop || !mTasks.empty(); });
                    if (mbStop && mTasks.empty()) {
                        return;
                    }
                    task = std::move(mTasks.front());
                    mTasks.pop();
                }

                task(); // 작업 실행
            }
        });
    }
}

// 소멸자
ThreadPoolManager::~ThreadPoolManager() 
{
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);
        mbStop = true;
    }
    mCondition.notify_all();
    for (std::thread& worker : mWorkers) {
        worker.join();
    }
}

// 작업을 큐에 추가하는 함수
void ThreadPoolManager::enqueue(std::function<void()> task) 
{
    {
        std::unique_lock<std::mutex> lock(mTasksMutex);
        if (mbStop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        mTasks.emplace(std::move(task));
    }
    mCondition.notify_one();
}

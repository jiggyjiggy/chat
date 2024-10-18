#ifndef THREADPOOLMANAGER_H
#define THREADPOOLMANAGER_H

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

class ThreadPoolManager {

public:
    ThreadPoolManager(size_t numThreads); // 생성자
    ~ThreadPoolManager(); // 소멸자

// 작업 추가
    void enqueue(std::function<void()> task); // 작업을 큐에 추가하는 함수

private:
    std::vector<std::thread> mWorkers; // 스레드 풀
    std::queue<std::function<void()>> mTasks; // 작업 큐
    std::mutex mTasksMutex; // 동기화용 뮤텍스
    std::condition_variable mCondition; // 조건 변수
    bool mbStop; // 스레드 풀 종료 여부

};

#endif // THREAD_POOL_MANAGER_H

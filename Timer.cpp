#include "framework.h"

// 构造函数，初始化并重置计时器
Timer::Timer() {
    Reset();
}

// 重置计时器
void Timer::Reset() {
    startTime = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration<float>::zero(); // 重置保存的时间
    running = true; 
}


float Timer::GetElapsedTime() {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        
        return std::chrono::duration<float>(now - startTime).count();
    }
    // 如果计时器已停止，则返回保存的时间
    return elapsedTime.count();
}

void Timer::Stop() {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        elapsedTime = now - startTime; // 保存停止时的时间
        running = false; 
    }
}


bool Timer::IsRunning() const {
    return running;
}

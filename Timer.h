#pragma once
class Timer {
public:
    Timer();

    // 重置计时器并开始
    void Reset();

    // 获取经过的时间（秒），如果计时器未停止则持续更新
    float GetElapsedTime();

    // 停止计时器
    void Stop();

    // 计时器是否在运行
    bool IsRunning() const;

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::duration<float> elapsedTime; // 用来保存停止时的时间
    bool running; // 用来跟踪计时器状态
};
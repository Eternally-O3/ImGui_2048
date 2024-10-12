#include "framework.h"

// ���캯������ʼ�������ü�ʱ��
Timer::Timer() {
    Reset();
}

// ���ü�ʱ��
void Timer::Reset() {
    startTime = std::chrono::high_resolution_clock::now();
    elapsedTime = std::chrono::duration<float>::zero(); // ���ñ����ʱ��
    running = true; 
}


float Timer::GetElapsedTime() {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        
        return std::chrono::duration<float>(now - startTime).count();
    }
    // �����ʱ����ֹͣ���򷵻ر����ʱ��
    return elapsedTime.count();
}

void Timer::Stop() {
    if (running) {
        auto now = std::chrono::high_resolution_clock::now();
        elapsedTime = now - startTime; // ����ֹͣʱ��ʱ��
        running = false; 
    }
}


bool Timer::IsRunning() const {
    return running;
}

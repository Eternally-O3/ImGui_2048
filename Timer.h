#pragma once
class Timer {
public:
    Timer();

    // ���ü�ʱ������ʼ
    void Reset();

    // ��ȡ������ʱ�䣨�룩�������ʱ��δֹͣ���������
    float GetElapsedTime();

    // ֹͣ��ʱ��
    void Stop();

    // ��ʱ���Ƿ�������
    bool IsRunning() const;

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::duration<float> elapsedTime; // ��������ֹͣʱ��ʱ��
    bool running; // �������ټ�ʱ��״̬
};
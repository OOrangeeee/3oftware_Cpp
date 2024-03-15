#include"tools.h"

int getRandomNumber(int a, int b) {
    // ʹ�õ�ǰʱ�����������Ϊ����
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    std::mt19937 gen(nanos); // ʹ���������ӳ�ʼ�������������
    std::uniform_int_distribution<> distrib(a, b); // ����������ֲ�

    return distrib(gen); // ���ɲ����������
}


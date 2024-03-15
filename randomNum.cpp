#include"tools.h"

int getRandomNumber(int a, int b) {
    // 使用当前时间的纳秒数作为种子
    auto now = std::chrono::high_resolution_clock::now();
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();

    std::mt19937 gen(nanos); // 使用纳秒种子初始化随机数生成器
    std::uniform_int_distribution<> distrib(a, b); // 定义随机数分布

    return distrib(gen); // 生成并返回随机数
}


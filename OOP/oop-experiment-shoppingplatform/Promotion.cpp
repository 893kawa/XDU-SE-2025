#include "Promotion.h"
#include <ctime>

// 促销基类构造函数
Promotion::Promotion(const std::string &t, const std::string &tg, time_t s, time_t e)
    : type(t), target(tg), start(s), end(e) {}

// 检查促销是否有效
bool Promotion::isActive() const
{
    time_t now = time(0);
    // 检查当前时间是否在促销时间范围内
    return difftime(now, start) >= 0 && difftime(end, now) >= 0;
}
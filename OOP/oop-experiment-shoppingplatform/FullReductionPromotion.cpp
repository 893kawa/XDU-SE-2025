#include "FullReductionPromotion.h"
#include <sstream>

// 满减促销构造函数
FullReductionPromotion::FullReductionPromotion(const std::string &targetType, double t, double r, time_t s, time_t e)
    : Promotion("FULL_REDUCE", targetType, s, e), threshold(t), reduction(r) {}

// 获取促销标签描述
std::string FullReductionPromotion::getLabel() const
{
    std::stringstream ss;
    ss << "满" << threshold << "减" << reduction; // 格式化为满减描述
    return ss.str();
}

// 应用满减到订单总价
double FullReductionPromotion::applyToOrder(double total) const
{
    // 满足条件时应用减免
    return (total >= threshold) ? (total - reduction) : total;
}

// 应用满减到商品总价
double FullReductionPromotion::applyToProductTotal(double total) const
{
    // 满足条件时应用减免
    return (total >= threshold) ? (total - reduction) : total;
}
#include "DiscountPromotion.h"
#include <sstream>

// 折扣促销构造函数
DiscountPromotion::DiscountPromotion(const std::string &product, double r, time_t s, time_t e)
    : Promotion("DISCOUNT", product, s, e), rate(r) {}

// 获取促销标签描述
std::string DiscountPromotion::getLabel() const
{
    std::stringstream ss;
    ss << "限时折扣 " << (rate * 10) << "折"; // 将小数转换为百分比折扣
    return ss.str();
}

// 应用折扣到单个商品
double DiscountPromotion::applyToProduct(double price, int qty) const
{
    return price * rate; // 计算折扣后价格
}

// 应用折扣到商品总价
double DiscountPromotion::applyToProductTotal(double total) const
{
    return total * rate; // 计算折扣后总价
}
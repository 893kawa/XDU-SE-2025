#ifndef DISCOUNT_PROMOTION_H
#define DISCOUNT_PROMOTION_H
#include "Promotion.h"

// 折扣促销类，继承自促销基类
class DiscountPromotion : public Promotion
{
    double rate; // 折扣率
public:
    // 构造函数
    DiscountPromotion(const std::string &product, double r, time_t s, time_t e);

    // 获取促销标签描述
    std::string getLabel() const override;

    // 应用折扣到单个商品
    double applyToProduct(double price, int qty) const override;

    // 应用折扣到商品总价
    double applyToProductTotal(double total) const override;

    // 获取折扣率
    double getRate() const { return rate; }

    // 设置折扣率
    void setRate(double r) { rate = r; }
};
#endif
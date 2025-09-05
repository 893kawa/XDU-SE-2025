#ifndef FULL_REDUCTION_PROMOTION_H
#define FULL_REDUCTION_PROMOTION_H
#include "Promotion.h"

// 满减促销类，继承自促销基类
class FullReductionPromotion : public Promotion
{
    double threshold; // 满减门槛金额
    double reduction; // 减免金额
public:
    // 构造函数
    FullReductionPromotion(const std::string &targetType, double t, double r, time_t s, time_t e);

    // 获取促销标签描述
    std::string getLabel() const override;

    // 应用满减到订单总价
    double applyToOrder(double total) const override;

    // 应用满减到商品总价
    double applyToProductTotal(double total) const override;

    // 获取满减门槛金额
    double getThreshold() const { return threshold; }

    // 设置满减门槛金额
    void setThreshold(double t) { threshold = t; }

    // 获取减免金额
    double getReduction() const { return reduction; }

    // 设置减免金额
    void setReduction(double r) { reduction = r; }
};
#endif
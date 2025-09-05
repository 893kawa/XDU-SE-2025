#ifndef PROMOTION_MANAGER_H
#define PROMOTION_MANAGER_H
#include <vector>
#include "Promotion.h"

// 促销管理器类
class PromotionManager
{
    std::vector<Promotion *> promotions; // 所有促销活动的容器
public:
    // 获取单例实例
    static PromotionManager &instance();

    // 添加促销
    void add(Promotion *p);

    // 获取商品促销列表
    std::vector<Promotion *> getProductPromotions(const std::string &product);

    // 获取订单促销列表
    std::vector<Promotion *> getOrderPromotions();

    // 获取所有折扣促销
    std::vector<Promotion *> getDiscountPromotions();

    // 获取商品满减促销
    std::vector<Promotion *> getFullReductionProductPromotions();

    // 获取订单满减促销
    std::vector<Promotion *> getFullReductionOrderPromotions();

    // 删除促销
    void removePromotion(Promotion *promo);

    // 查找促销
    Promotion *findPromotion(const std::string &product, const std::string &type);

    // 修改促销参数
    void modifyPromotion(Promotion *promo, double value1, double value2);
};
#endif
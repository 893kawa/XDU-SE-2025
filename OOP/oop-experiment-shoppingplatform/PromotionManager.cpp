#include "PromotionManager.h"
#include <algorithm>
#include "DiscountPromotion.h"
#include "FullReductionPromotion.h"

// 单例实例获取
PromotionManager &PromotionManager::instance()
{
    static PromotionManager mgr; // 静态局部变量确保唯一实例
    return mgr;
}

// 添加促销活动
void PromotionManager::add(Promotion *p)
{
    promotions.push_back(p); // 添加到促销列表
}

// 获取特定商品的促销活动
std::vector<Promotion *> PromotionManager::getProductPromotions(const std::string &product)
{
    std::vector<Promotion *> result;
    // 遍历所有促销
    for (auto *p : promotions)
    {
        // 检查是否针对该商品且有效
        if (p->getTarget() == product && p->isActive())
            result.push_back(p); // 添加到结果列表
    }
    return result;
}

// 获取订单级促销活动
std::vector<Promotion *> PromotionManager::getOrderPromotions()
{
    std::vector<Promotion *> result;
    // 遍历所有促销
    for (auto *p : promotions)
    {
        // 检查是否订单级促销且有效
        if (p->getTarget() == "ORDER" && p->isActive())
            result.push_back(p); // 添加到结果列表
    }
    return result;
}

// 获取所有折扣促销
std::vector<Promotion *> PromotionManager::getDiscountPromotions()
{
    std::vector<Promotion *> result;
    // 遍历所有促销
    for (auto *p : promotions)
    {
        // 检查是否为折扣促销且有效
        if (dynamic_cast<DiscountPromotion *>(p) && p->isActive())
            result.push_back(p); // 添加到结果列表
    }
    return result;
}

// 获取商品级满减促销
std::vector<Promotion *> PromotionManager::getFullReductionProductPromotions()
{
    std::vector<Promotion *> result;
    // 遍历所有促销
    for (auto *p : promotions)
    {
        // 检查是否为满减促销、非订单级且有效
        if (dynamic_cast<FullReductionPromotion *>(p) && p->getTarget() != "ORDER" && p->isActive())
            result.push_back(p); // 添加到结果列表
    }
    return result;
}

// 获取订单级满减促销
std::vector<Promotion *> PromotionManager::getFullReductionOrderPromotions()
{
    std::vector<Promotion *> result;
    // 遍历所有促销
    for (auto *p : promotions)
    {
        // 检查是否为满减促销、订单级且有效
        if (dynamic_cast<FullReductionPromotion *>(p) && p->getTarget() == "ORDER" && p->isActive())
            result.push_back(p); // 添加到结果列表
    }
    return result;
}

// 删除促销活动
void PromotionManager::removePromotion(Promotion *promo)
{
    // 在列表中查找促销
    auto it = std::find(promotions.begin(), promotions.end(), promo);
    if (it != promotions.end())
    {
        promotions.erase(it); // 从列表中移除
        delete promo;         // 释放内存
    }
}

// 查找特定商品的特定类型促销
Promotion *PromotionManager::findPromotion(const std::string &product, const std::string &type)
{
    for (auto *p : promotions)
    {
        // 检查目标商品和类型是否匹配
        if (p->getTarget() == product && p->getType() == type)
        {
            return p; // 返回促销指针
        }
    }
    return nullptr; // 未找到促销
}

// 修改促销参数
void PromotionManager::modifyPromotion(Promotion *promo, double value1, double value2)
{
    // 尝试转换为折扣促销
    if (auto discount = dynamic_cast<DiscountPromotion *>(promo))
    {
        discount->setRate(value1); // 设置折扣率
    }
    // 尝试转换为满减促销
    else if (auto fullReduction = dynamic_cast<FullReductionPromotion *>(promo))
    {
        fullReduction->setThreshold(value1); // 设置满减门槛
        fullReduction->setReduction(value2); // 设置减免金额
    }
}
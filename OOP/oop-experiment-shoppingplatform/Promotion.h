#ifndef PROMOTION_H
#define PROMOTION_H
#include <ctime>
#include <string>

// 促销基类
class Promotion
{
protected:
    std::string type;   // 促销类型
    std::string target; // 促销目标（商品名或"ORDER"）
    time_t start;       // 促销开始时间
    time_t end;         // 促销结束时间

public:
    // 构造函数
    Promotion(const std::string &t, const std::string &tg, time_t s, time_t e);
    virtual ~Promotion() {} // 虚析构函数

    // 检查促销是否有效
    virtual bool isActive() const;

    // 获取促销标签（纯虚函数）
    virtual std::string getLabel() const = 0;

    // 应用到单个商品（默认不改变价格）
    virtual double applyToProduct(double price, int qty) const { return price; }

    // 应用到订单总价（默认不改变价格）
    virtual double applyToOrder(double total) const { return total; }

    // 应用到商品总价（默认不改变价格）
    virtual double applyToProductTotal(double total) const { return total; }

    // 获取促销目标
    std::string getTarget() const { return target; }

    // 获取促销类型
    std::string getType() const { return type; }
};
#endif
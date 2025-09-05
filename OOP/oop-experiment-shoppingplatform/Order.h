#ifndef ORDER_H
#define ORDER_H

#include "Product.h"
#include <vector>
#include <string>
#include <ctime>

// 订单类
class Order
{
private:
    std::string orderId;                            // 订单ID
    std::string username;                           // 下单用户名
    std::vector<std::pair<std::string, int>> items; // 商品名称和数量
    time_t createTime;                              // 订单创建时间
    time_t shipTime;                                // 发货时间
    double totalAmount;                             // 订单总金额
    std::string address;                            // 收货地址
    std::string status;                             // 订单状态

public:
    Order() = default; // 默认构造函数

    // 参数化构造函数
    Order(const std::string &user,
          const std::vector<std::pair<std::string, int>> &items,
          double total,
          const std::string &addr);

    // Getter方法
    std::string getOrderId() const { return orderId; }
    std::string getUsername() const { return username; }
    time_t getCreateTime() const { return createTime; }
    time_t getShipTime() const { return shipTime; }
    double getTotal() const { return totalAmount; }
    std::string getAddress() const { return address; }
    std::string getStatus() const { return status; }
    const std::vector<std::pair<std::string, int>> &getItems() const { return items; }

    // Setter方法
    void setOrderId(const std::string &id) { orderId = id; }
    void setUsername(const std::string &name) { username = name; }
    void setTotal(double total) { totalAmount = total; }
    void setAddress(const std::string &addr) { address = addr; }
    void setStatus(const std::string &s) { status = s; }
    void setCreateTime(time_t t) { createTime = t; }
    void setShipTime(time_t t) { shipTime = t; }

    // 添加商品到订单
    void addItem(const std::string &productId, int quantity)
    {
        items.emplace_back(productId, quantity);
    }

    // 状态操作
    void autoUpdateStatus();                              // 自动更新订单状态
    bool canModify() const { return status == "待发货"; } // 检查订单是否可修改

    // 库存恢复
    void restoreStock() const; // 取消订单时恢复库存

    static int orderCounter; // 静态计数器，用于生成唯一订单ID

    void printSummary() const; // 打印订单基本信息及商品详情
};

#endif
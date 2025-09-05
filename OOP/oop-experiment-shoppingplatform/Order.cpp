#define _CRT_SECURE_NO_WARNINGS
#include "Order.h"
#include "ProductList.h"
#include <sstream>
#include <iostream>
#include <iomanip> // 用于格式化输出
#include <ctime>   // 时间处理

// 静态计数器初始化
int Order::orderCounter = 0;

// 订单构造函数
Order::Order(const std::string &user,
             const std::vector<std::pair<std::string, int>> &items,
             double total,
             const std::string &addr)
    : username(user), items(items), totalAmount(total), address(addr)
{

    // 生成唯一订单ID（时间戳+计数器）
    std::stringstream ss;
    ss << time(0) << "-" << (++orderCounter);
    orderId = ss.str();

    createTime = time(0); // 设置创建时间
    shipTime = 0;         // 初始发货时间为0
    status = "待支付";    // 初始状态
}

// 自动更新订单状态
void Order::autoUpdateStatus()
{
    time_t now = time(0);
    // 超过15秒未支付自动变为"已支付"
    if (status == "待支付" && difftime(now, createTime) > 15)
    {
        status = "已支付";
        shipTime = now; // 记录支付时间
    }
    // 支付后超过10秒自动变为"已发货"
    else if (status == "已支付" && difftime(now, shipTime) > 10)
    {
        status = "已发货";
    }
}

// 恢复库存（取消订单时调用）
void Order::restoreStock() const
{
    for (auto &item : items)
    {
        ProductList::updateStock(item.first, item.second); // 恢复商品库存
    }
}

// 打印订单摘要信息
void Order::printSummary() const
{
    std::cout << "-----------------------------\n";
    std::cout << "订单号: " << orderId << "\n";
    std::cout << "状态: " << status << "\n";
    std::cout << "地址: " << address << "\n";
    std::cout << "总金额: " << totalAmount << "\n";

    // 格式化输出创建时间
    std::tm *timeinfo = localtime(&createTime);
    std::cout << "创建时间: " << std::put_time(timeinfo, "%Y-%m-%d %H:%M:%S") << "\n";

    std::cout << "商品:";
    // 输出订单商品列表
    for (const auto &item : items)
    {
        std::cout << "  - " << item.first << " x" << item.second << "\n";
    }
    std::cout << "-----------------------------\n";
}
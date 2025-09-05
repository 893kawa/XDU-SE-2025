#include "OrderList.h"
#include "ProductList.h"
#include "PromotionManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

// 静态成员初始化
std::vector<Order> OrderList::orders;

// 保存订单到文件
void OrderList::saveOrders(const std::string &filename)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        for (const auto &order : orders)
        {
            // 写入订单基本信息
            file << order.getOrderId() << ","
                 << order.getUsername() << ","
                 << order.getCreateTime() << ","
                 << order.getShipTime() << ","
                 << order.getTotal() << ","
                 << order.getAddress() << ","
                 << order.getStatus() << ",";

            // 写入订单商品信息
            const auto &items = order.getItems();
            for (const auto &item : items)
            {
                file << item.first << ":" << item.second << ";";
            }
            file << "\n";
        }
        file.close();
    }
    else
    {
        std::cerr << "无法打开文件 " << filename << " 进行写入。" << std::endl;
    }
}

// 从文件加载订单
void OrderList::loadOrders(const std::string &filename)
{
    std::ifstream file(filename);
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string orderId, username, address, status;
            time_t createTime, shipTime;
            double total;

            // 解析订单基本信息
            std::getline(iss, orderId, ',');
            std::getline(iss, username, ',');
            iss >> createTime;
            iss.ignore();
            iss >> shipTime;
            iss.ignore();
            iss >> total;
            iss.ignore();
            std::getline(iss, address, ',');
            std::getline(iss, status, ',');

            Order order;
            order.setOrderId(orderId);
            order.setUsername(username);
            order.setCreateTime(createTime);
            order.setShipTime(shipTime);
            order.setTotal(total);
            order.setAddress(address);
            order.setStatus(status);

            // 解析订单商品信息
            std::string itemStr;
            while (std::getline(iss, itemStr, ';'))
            {
                if (!itemStr.empty())
                {
                    std::istringstream itemIss(itemStr);
                    std::string productName;
                    int quantity;
                    std::getline(itemIss, productName, ':');
                    itemIss >> quantity;
                    order.addItem(productName, quantity); // 添加商品到订单
                }
            }

            orders.push_back(order); // 添加到订单列表
        }
        file.close();
    }
    else
    {
        std::cerr << "无法打开文件 " << filename << " 进行读取。" << std::endl;
    }
}

// 添加新订单
void OrderList::addOrder(const Order &order)
{
    orders.push_back(order);
    std::cout << "订单创建成功，订单号：" << order.getOrderId() << "\n";
}

// 删除订单
bool OrderList::removeOrder(const std::string &orderId, const std::string &username)
{
    for (auto it = orders.begin(); it != orders.end(); ++it)
    {
        if (it->getOrderId() == orderId && it->getUsername() == username)
        {
            orders.erase(it);                    // 从列表中删除
            OrderList::saveOrders("orders.txt"); // 保存更改
            return true;                         // 删除成功
        }
    }
    return false; // 未找到订单
}

// 获取用户的订单列表
std::vector<Order> OrderList::getByUser(const std::string &username)
{
    std::vector<Order> result;
    for (const auto &order : orders)
    {
        if (order.getUsername() == username)
        {
            result.push_back(order); // 添加到结果列表
        }
    }
    return result;
}

// 查找订单
Order *OrderList::findOrder(const std::string &orderId)
{
    for (auto &order : orders)
    {
        if (order.getOrderId() == orderId)
        {
            return &order; // 返回订单指针
        }
    }
    return nullptr; // 未找到订单
}

// 自动更新所有订单状态
void OrderList::autoUpdateAllStatuses()
{
    for (auto &order : orders)
    {
        order.autoUpdateStatus(); // 更新单个订单状态
    }
    OrderList::saveOrders("orders.txt"); // 保存状态更新
}

// 创建新订单（带促销应用）
void OrderList::createOrder(const std::string &username,
                            const std::vector<std::pair<std::string, int>> &items,
                            const std::string &address)
{
    double total = 0; // 订单总金额

    // 第一阶段：计算商品小计并应用商品促销
    for (auto &item : items)
    {
        Product *p = ProductList::findProductByName(item.first);
        double price = p->getPrice();
        int qty = item.second;

        double productTotal = price * qty; // 商品小计

        // 应用商品促销（如折扣）
        for (auto *promo : PromotionManager::instance().getProductPromotions(p->getName()))
        {
            if (promo->isActive())
                productTotal = promo->applyToProductTotal(productTotal);
        }
        total += productTotal; // 累加到总金额
    }

    // 第二阶段：应用订单促销（如满减）
    for (auto *promo : PromotionManager::instance().getOrderPromotions())
    {
        if (promo->isActive())
            total = promo->applyToOrder(total);
    }

    // 检查库存并扣减
    for (const auto &item : items)
    {
        Product *p = ProductList::findProductByName(item.first);
        if (p && p->getStock() >= item.second)
        {
            ProductList::updateStock(item.first, -item.second); // 扣减库存
        }
        else
        {
            std::cout << "商品 " << item.first << " 库存不足，订单创建失败。\n";
            return;
        }
    }

    // 创建订单对象并添加到系统
    Order order(username, items, total, address);
    addOrder(order);
    OrderList::saveOrders("orders.txt"); // 保存订单
}

// 管理员更新订单状态
bool OrderList::adminUpdateStatus(const std::string &orderId, const std::string &newStatus)
{
    for (auto &order : orders)
    {
        if (order.getOrderId() == orderId)
        {
            order.setStatus(newStatus); // 更新状态
            std::cout << "订单 " << orderId << " 的状态已更新为：" << newStatus << "\n";
            OrderList::saveOrders("orders.txt"); // 保存更改
            return true;                         // 更新成功
        }
    }
    std::cout << "未找到订单号为 " << orderId << " 的订单。" << std::endl;
    return false; // 未找到订单
}
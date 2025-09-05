#ifndef ORDERLIST_H
#define ORDERLIST_H

#include "Order.h"
#include <vector>

class OrderList {
private:
    static std::vector<Order> orders;
public:
    static bool adminUpdateStatus(const std::string& orderId, const std::string& newStatus);
    static void addOrder(const Order& order);
    static bool removeOrder(const std::string& orderId, const std::string& username);
    static std::vector<Order> getByUser(const std::string& username);
    static Order* findOrder(const std::string& orderId);
    static void autoUpdateAllStatuses();
    static void createOrder(const std::string& username,
        const std::vector<std::pair<std::string, int>>& items,
        const std::string& address);
    static void loadOrders(const std::string& filename); // 新增声明
    static void saveOrders(const std::string& filename); // 新增声明
};

#endif
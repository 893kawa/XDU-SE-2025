#include "ShoppingCart.h"
#include "ProductList.h"
#include "Customer.h"
#include "Order.h"
#include "OrderList.h"
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

// 静态成员初始化（用户购物车映射）
map<string, vector<CartItem>> ShoppingCart::userCarts;

// 从文件加载购物车数据
void ShoppingCart::loadCart()
{
    ifstream ifs("shopping_cart.txt");
    if (ifs.is_open())
    {
        string user, productName;
        int quantity;
        // 读取购物车数据
        while (ifs >> user >> productName >> quantity)
        {
            userCarts[user].push_back({productName, quantity}); // 添加到用户购物车
        }
        ifs.close();
    }
}

// 保存购物车数据到文件
void ShoppingCart::saveCart()
{
    ofstream ofs("shopping_cart.txt");
    if (ofs.is_open())
    {
        // 遍历所有用户的购物车
        for (const auto &userCart : userCarts)
        {
            const string &user = userCart.first;
            const vector<CartItem> &items = userCart.second;
            // 写入用户购物车数据
            for (const auto &item : items)
            {
                ofs << user << " " << item.productName << " " << item.quantity << endl;
            }
        }
        ofs.close();
    }
}

// 添加商品到购物车
bool ShoppingCart::addItem(const string &productName, int quantity)
{
    // 检查登录状态
    if (!Customer::isLoggedIn())
    {
        cout << "请先登录！\n";
        return false;
    }

    // 检查商品是否存在
    Product *product = ProductList::findProductByName(productName);
    if (!product)
    {
        cout << "商品不存在！\n";
        return false;
    }

    string user = Customer::getCurrentUser();
    vector<CartItem> &items = userCarts[user]; // 获取当前用户购物车

    // 检查商品是否已在购物车
    auto it = find_if(items.begin(), items.end(),
                      [&](const CartItem &item)
                      { return item.productName == productName; });

    if (it != items.end())
    {
        // 商品已存在，提供操作选项
        cout << "商品已在购物车中，当前数量为" << it->quantity
             << "\n1. 增加数量\n2. 直接修改数量\n请选择操作：";
        int op;
        cin >> op;
        if (op == 1)
        {
            it->quantity += quantity; // 增加数量
        }
        else if (op == 2)
        {
            cout << "请输入新的数量：";
            cin >> it->quantity; // 修改数量
        }
    }
    else
    {
        // 商品不存在，添加新项
        items.push_back({productName, quantity});
    }
    saveCart(); // 保存购物车
    return true;
}

// 从购物车移除商品
bool ShoppingCart::removeItem(const vector<string> &productNames)
{
    // 检查登录状态
    if (!Customer::isLoggedIn())
    {
        cout << "请先登录！\n";
        return false;
    }

    string user = Customer::getCurrentUser();
    auto &items = userCarts[user]; // 获取当前用户购物车
    size_t originalSize = items.size();

    // 移除指定商品
    items.erase(remove_if(items.begin(), items.end(),
                          [&productNames](const CartItem &item)
                          {
                              return find(productNames.begin(), productNames.end(), item.productName) != productNames.end();
                          }),
                items.end());

    int removedCount = originalSize - items.size(); // 计算移除数量
    cout << "成功移除 " << removedCount << " 个商品\n";
    saveCart();              // 保存购物车
    return removedCount > 0; // 返回是否移除了商品
}

// 更新购物车商品数量
bool ShoppingCart::updateQuantity(const string &productName, int newQuantity)
{
    // 检查登录状态
    if (!Customer::isLoggedIn())
    {
        cout << "请先登录！\n";
        return false;
    }

    string user = Customer::getCurrentUser();
    auto &items = userCarts[user]; // 获取当前用户购物车

    // 查找商品
    auto it = find_if(items.begin(), items.end(),
                      [&productName](const CartItem &item)
                      {
                          return item.productName == productName;
                      });

    if (it == items.end())
    {
        cout << "未找到商品: " << productName << endl;
        return false; // 商品不存在
    }

    if (newQuantity <= 0)
    {
        // 数量为0或负数，询问是否删除
        cout << "是否要删除该商品？(1: 是, 其他: 否)：";
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            items.erase(it); // 移除商品
            cout << "商品已移除" << endl;
        }
        else
        {
            cout << "取消修改，当前数量不变。" << endl;
        }
    }
    else
    {
        it->quantity = newQuantity; // 更新数量
        cout << "数量已更新为: " << newQuantity << endl;
    }
    saveCart(); // 保存购物车
    return true;
}

// 显示购物车内容
void ShoppingCart::displayCart()
{
    // 检查登录状态
    if (!Customer::isLoggedIn())
    {
        cout << "请先登录！\n";
        return;
    }

    string user = Customer::getCurrentUser();
    auto &items = userCarts[user]; // 获取当前用户购物车

    // 检查购物车是否为空
    if (items.empty())
    {
        cout << "购物车为空！\n";
        return;
    }

    double totalPrice = 0.0; // 总价格
    int totalItems = 0;      // 总数量

    cout << "\n[购物车详情]\n";
    // 遍历购物车商品
    for (const auto &item : items)
    {
        Product *product = ProductList::findProductByName(item.productName);
        if (product)
        {
            // 计算商品小计
            double subtotal = product->getPrice() * item.quantity;
            cout << "商品名称: " << item.productName
                 << "\n  类别: " << product->getCategory()
                 << "\n  价格: " << product->getPrice()
                 << "\n  数量: " << item.quantity
                 << "\n  小计: " << subtotal << "\n";
            totalPrice += subtotal;      // 累加总价
            totalItems += item.quantity; // 累加总数
        }
        else
        {
            cout << "注意：商品 '" << item.productName << "' 已下架\n";
        }
    }

    // 输出总计信息
    cout << "总计：\n"
         << "  总数量: " << totalItems << "\n"
         << "  总价: " << totalPrice << "\n";
}

// 获取当前购物车内容
vector<CartItem> ShoppingCart::getCurrentCart()
{
    string user = Customer::getCurrentUser();
    if (user.empty())
        return {};          // 未登录返回空
    return userCarts[user]; // 返回购物车内容
}

// 清空购物车
void ShoppingCart::clearCart()
{
    string user = Customer::getCurrentUser();
    if (!user.empty())
    {
        userCarts[user].clear(); // 清空当前用户购物车
        saveCart();              // 保存更改
    }
}

// 结账并创建订单
bool ShoppingCart::checkout(const string &address)
{
    string user = Customer::getCurrentUser();
    if (user.empty())
        return false; // 未登录

    vector<CartItem> items = getCurrentCart();
    vector<pair<string, int>> orderItems; // 订单商品列表
    double total = 0.0;                   // 订单总价

    // 检查库存并准备订单数据
    for (auto &item : items)
    {
        Product *p = ProductList::findProductByName(item.productName);
        if (!p || p->getStock() < item.quantity)
        {
            cout << "商品 " << item.productName << " 库存不足！\n";
            return false; // 库存不足
        }
        total += p->getPrice() * item.quantity; // 计算总价
        orderItems.push_back({item.productName, item.quantity});
    }

    // 创建订单
    Order newOrder(user, orderItems, total, address);
    OrderList::addOrder(newOrder);

    // 清空购物车
    clearCart();
    return true; // 结账成功
}
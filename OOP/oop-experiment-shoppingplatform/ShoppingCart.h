#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

#include "Product.h"
#include <vector>
#include <map>
using namespace std;

// 购物车项结构
struct CartItem
{
    string productName; // 商品名称
    int quantity;       // 商品数量
};

// 购物车管理类
class ShoppingCart
{
private:
    // 静态成员：用户购物车映射（用户名->购物车项列表）
    static map<string, vector<CartItem>> userCarts;

public:
    // 添加商品到购物车
    static bool addItem(const string &productName, int quantity);

    // 从购物车移除商品
    static bool removeItem(const vector<string> &productNames);

    // 更新购物车商品数量
    static bool updateQuantity(const string &productName, int newQuantity);

    // 显示购物车内容
    static void displayCart();

    // 结账
    static bool checkout(const string &address);

    // 获取当前购物车内容
    static vector<CartItem> getCurrentCart();

    // 清空购物车
    static void clearCart();

    // 从文件加载购物车数据
    static void loadCart();

    // 保存购物车数据到文件
    static void saveCart();
};

#endif
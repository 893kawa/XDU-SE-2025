#include <iostream>
#include <limits>
#include <sstream>
#include "Admin.h"
#include "Customer.h"
#include "ProductList.h"
#include "ShoppingCart.h"
#include "Order.h"
#include "OrderList.h"
#include "PromotionManager.h"
#include "Promotion.h"
#include "FullReductionPromotion.h"
#include "DiscountPromotion.h"
using namespace std;

// 促销管理菜单
void menuPromotionAdmin()
{
    int choice;
    int delta = 0; // 促销持续时间（秒）
    while (true)
    {
        cout << "\n[促销管理]\n1. 添加限时折扣\n2. 添加满减\n3. 展示商品折扣列表\n4. 展示商品满减列表\n5. 查看订单满减\n6. 删除促销活动\n7. 修改促销活动\n0. 返回\n选择：";
        cin >> choice;
        if (choice == 0)
            break; // 返回上级菜单

        string product;
        double v1, v2;

        switch (choice)
        {
        case 1:
        { // 添加限时折扣
            cout << "商品名 折扣率 时限（秒）: ";
            cin >> product >> v1 >> delta;
            time_t start = time(0), end = start + delta; // 计算结束时间
            PromotionManager::instance().add(
                new DiscountPromotion(product, v1, start, end)); // 创建并添加折扣促销
            break;
        }
        case 2:
        { // 添加满减
            cout << "促销类型（PRODUCT/ORDER） 满额 减额 时限（秒）: ";
            string type;
            cin >> type >> v1 >> v2 >> delta;
            time_t start = time(0), end = start + delta; // 计算结束时间
            PromotionManager::instance().add(
                new FullReductionPromotion(type, v1, v2, start, end)); // 创建并添加满减促销
            break;
        }
        case 3:
        { // 展示商品折扣列表
            auto discounts = PromotionManager::instance().getDiscountPromotions();
            for (auto *promo : discounts)
            {
                cout << "商品: " << promo->getTarget() << ", 促销: " << promo->getLabel() << endl;
            }
            break;
        }
        case 4:
        { // 展示商品满减列表
            auto fullReductions = PromotionManager::instance().getFullReductionProductPromotions();
            for (auto *promo : fullReductions)
            {
                cout << "商品: " << promo->getTarget() << ", 促销: " << promo->getLabel() << endl;
            }
            break;
        }
        case 5:
        { // 查看订单满减
            auto fullReductions = PromotionManager::instance().getFullReductionOrderPromotions();
            for (auto *promo : fullReductions)
            {
                cout << "促销: " << promo->getLabel() << endl;
            }
            break;
        }
        case 6:
        { // 删除促销活动
            cout << "商品名 促销类型（DISCOUNT/FULL_REDUCE）: ";
            string product, type;
            cin >> product >> type;
            Promotion *promo = PromotionManager::instance().findPromotion(product, type);
            if (!promo)
            {
                cout << "未找到该促销活动。" << endl;
            }
            else
            {
                PromotionManager::instance().removePromotion(promo); // 删除促销
                cout << "促销活动已删除。" << endl;
            }
            break;
        }
        case 7:
        { // 修改促销活动
            cout << "商品名 促销类型（DISCOUNT/FULL_REDUCE）: ";
            string product, type;
            cin >> product >> type;
            Promotion *promo = PromotionManager::instance().findPromotion(product, type);
            if (promo)
            {
                if (type == "DISCOUNT")
                {
                    cout << "新的折扣率: ";
                    cin >> v1;
                    PromotionManager::instance().modifyPromotion(promo, v1, 0); // 修改折扣率
                }
                else if (type == "FULL_REDUCE")
                {
                    cout << "新的满额 新的减额: ";
                    cin >> v1 >> v2;
                    PromotionManager::instance().modifyPromotion(promo, v1, v2); // 修改满减参数
                }
            }
            else
            {
                cout << "未找到该促销活动。" << endl;
            }
            break;
        }
        }
    }
}

// 商品管理菜单
void menuProductAdmin()
{
    int choice;
    while (true)
    {
        cout << "\n[商品管理菜单]\n"
             << "1. 添加商品\n"
             << "2. 修改商品\n"
             << "3. 删除商品\n"
             << "4. 查看所有商品\n"
             << "5. 查询商品（精确匹配）\n"
             << "6. 查询商品（模糊匹配）\n"
             << "0. 返回\n选择：";
        cin >> choice;
        if (choice == 0)
            break; // 返回上级菜单

        string cat, name, desc, keyword;
        double price;
        int stock;

        switch (choice)
        {
        case 1: // 添加商品
            cout << "商品名称：";
            cin >> name;
            cout << "类别：";
            cin >> cat;
            cout << "价格：";
            cin >> price;
            cout << "描述：";
            cin.ignore();
            getline(cin, desc);
            cout << "库存：";
            cin >> stock;
            ProductList::addProduct(Product(cat, name, price, desc, stock)); // 添加新商品
            break;
        case 2: // 修改商品
            cout << "输入要修改的商品名称：";
            cin >> name;
            ProductList::modifyProduct(name); // 调用修改函数
            break;
        case 3: // 删除商品
            cout << "输入要删除的商品名称：";
            cin >> name;
            ProductList::deleteProduct(name); // 调用删除函数
            break;
        case 4:                     // 查看所有商品
            ProductList::showAll(); // 显示所有商品
            break;
        case 5: // 精确查询
            cout << "输入查询关键字（名称/类别/价格）：";
            cin >> keyword;
            ProductList::searchExact(keyword); // 精确匹配查询
            break;
        case 6: // 模糊查询
            cout << "输入查询关键字（名称/类别/价格）：";
            cin >> keyword;
            ProductList::searchFuzzy(keyword); // 模糊匹配查询
            break;
        default:
            cout << "无效选择。\n";
        }
    }
}

// 商品查询菜单
void menuQueryProduct()
{
    int choice;

    while (true)
    {
        cout << "\n[商品查询菜单]\n"
             << "1. 查看所有商品\n"
             << "2. 精确匹配查询\n"
             << "3. 模糊匹配查询\n"
             << "4. 购买商品\n"
             << "0. 返回\n"
             << "选择：";
        cin >> choice;

        vector<pair<string, int>> items; // 存储购买商品列表
        string address;                  // 收货地址
        string name;                     // 商品名称
        int quantity;                    // 购买数量

        if (choice == 0)
            break; // 返回上级菜单

        switch (choice)
        {
        case 1:                     // 查看所有商品
            ProductList::showAll(); // 显示所有商品
            break;

        case 2:
        { // 精确匹配查询
            string keyword;
            cout << "输入查询关键字（名称/类别/价格）：";
            cin >> keyword;
            ProductList::searchExact(keyword); // 执行精确查询
            break;
        }

        case 3:
        { // 模糊匹配查询
            string keyword;
            cout << "输入查询关键字（名称/类别/价格）：";
            cin >> keyword;
            ProductList::searchFuzzy(keyword); // 执行模糊查询
            break;
        }

        case 4:
        { // 购买商品
            if (!Customer::isLoggedIn())
            {
                cout << "请先登录！\n";
                break;
            }

            // 显示所有商品
            ProductList::showAll();

            // 获取用户输入
            cout << "输入商品名称：";
            cin >> name;
            cout << "输入购买数量：";
            cin >> quantity;

            // 获取收货地址
            cout << "输入收货地址：";
            cin.ignore(); // 清除输入缓冲区
            getline(cin, address);

            // 准备订单数据
            items.push_back({name, quantity});

            // 创建订单
            OrderList::createOrder(Customer::getCurrentUser(), items, address);
            OrderList::autoUpdateAllStatuses(); // 更新订单状态
            break;
        }
        default:
            cout << "无效选择！\n";
        }
    }
}

// 订单管理菜单（管理员）
void menuOrderAdmin()
{
    int choice;
    string orderId;
    string newStatus;
    while (true)
    {
        std::cout << "\n[订单管理]\n";
        std::string orderId, newStatus;
        std::cout << "请输入要修改状态的订单号：\n";
        std::cin >> orderId;
        std::cout << "请输入新的订单状态（如：已发货 / 已签收 / 其他）：\n";
        std::cin >> newStatus;
        OrderList::adminUpdateStatus(orderId, newStatus); // 更新订单状态
        break;
    }
}

// 管理员主菜单
void menuAdmin()
{
    int choice;
    string u, p;
    while (true)
    {
        cout << "\n[管理员菜单]\n"
             << "1. 显示所有顾客\n"
             << "2. 重置顾客密码\n"
             << "3. 商品管理\n"
             << "4. 订单管理\n"
             << "5. 促销管理\n"
             << "0. 返回\n选择：";
        cin >> choice;
        if (choice == 0)
            break; // 返回主菜单

        switch (choice)
        {
        case 1: // 显示所有顾客
            Admin::showAllCustomers();
            break;
        case 2: // 重置密码
            cout << "顾客用户名：";
            cin >> u;
            cout << "新密码：";
            cin >> p;
            if (Admin::resetCustomerPassword(u, p))
                cout << "密码重置成功。\n";
            else
                cout << "用户不存在。\n";
            break;
        case 3: // 商品管理
            menuProductAdmin();
            break;
        case 4: // 订单管理
            menuOrderAdmin();
            break;
        case 5: // 促销管理
            menuPromotionAdmin();
            break;
        default:
            cout << "无效选择。\n";
        }
    }
}

// 购物车管理菜单
void menuShoppingCart()
{
    int choice;

    while (true)
    {
        cout << "\n[购物车管理]\n"
             << "1. 添加商品\n"
             << "2. 移除商品\n"
             << "3. 修改数量\n"
             << "4. 查看购物车\n"
             << "5. 购买购物车中的商品\n"
             << "0. 返回\n选择：";
        cin >> choice;

        // 将变量声明移动到 switch 语句外部
        std::vector<std::pair<std::string, int>> items; // 订单商品列表
        std::string address;                            // 收货地址

        if (choice == 0)
            break; // 返回上级菜单

        switch (choice)
        {
        case 1:
        {                           // 添加商品到购物车
            ProductList::showAll(); // 显示所有商品
            string name;
            int qty;
            cout << "输入商品名称：";
            cin >> name;
            cout << "输入数量：";
            cin >> qty;
            ShoppingCart::addItem(name, qty); // 添加商品
            break;
        }
        case 2:
        { // 从购物车移除商品
            string name;
            cout << "输入要移除的商品名称：";
            cin >> name;
            ShoppingCart::removeItem({name}); // 移除商品
            break;
        }
        case 3:
        { // 修改购物车商品数量
            string name;
            int qty;
            cout << "输入商品名称：";
            cin >> name;
            cout << "输入新数量：";
            cin >> qty;
            ShoppingCart::updateQuantity(name, qty); // 更新数量
            break;
        }
        case 4:                          // 查看购物车
            ShoppingCart::displayCart(); // 显示购物车内容
            break;
        case 5:
        { // 购买购物车商品
            if (!Customer::isLoggedIn())
            {
                cout << "请先登录！\n";
                break;
            }

            vector<CartItem> cartItems = ShoppingCart::getCurrentCart();
            if (cartItems.empty())
            {
                cout << "购物车为空！\n";
                break;
            }

            // 转换购物车数据为订单数据
            for (const auto &cartItem : cartItems)
            {
                items.push_back({cartItem.productName, cartItem.quantity});
            }

            cout << "输入收货地址：";
            cin.ignore();
            getline(cin, address);

            // 创建订单
            OrderList::autoUpdateAllStatuses();
            OrderList::createOrder(Customer::getCurrentUser(), items, address);
            ShoppingCart::clearCart(); // 清空购物车
            break;
        }
        default:
            cout << "无效选择！\n";
        }
    }
}

// 订单管理菜单（顾客）
void menuOrderManagement()
{
    // 检查登录状态
    if (!Customer::isLoggedIn())
    {
        cout << "请先登录！\n";
        return;
    }

    int choice;
    while (true)
    {
        cout << "\n[订单管理]\n"
             << "1. 查看所有订单\n"
             << "2. 修改收货地址\n"
             << "3. 取消订单\n"
             << "4. 删除订单\n"
             << "0. 返回\n选择：";
        cin >> choice;

        if (choice == 0)
            break; // 返回上级菜单

        string orderId, newAddr;
        switch (choice)
        {
        case 1:
        {                                       // 查看所有订单
            OrderList::autoUpdateAllStatuses(); // 更新订单状态
            std::vector<Order> orders = OrderList::getByUser(Customer::getCurrentUser());
            if (orders.empty())
            {
                std::cout << "您暂无订单记录。\n";
            }
            else
            {
                for (const auto &order : orders)
                {
                    order.printSummary(); // 打印订单摘要
                }
            }
            break;
        }
        case 2:
        { // 修改收货地址
            cout << "输入订单号：";
            cin >> orderId;
            OrderList::autoUpdateAllStatuses();
            Order *o = OrderList::findOrder(orderId);
            if (o && o->getUsername() == Customer::getCurrentUser())
            {
                if (o->canModify())
                { // 检查订单是否可修改
                    cout << "新地址：";
                    cin.ignore();
                    getline(cin, newAddr);
                    o->setAddress(newAddr); // 更新地址
                }
                else
                {
                    cout << "订单不可修改！\n";
                }
            }
            break;
        }
        case 3:
        { // 取消订单
            cout << "输入订单号：";
            cin >> orderId;
            OrderList::autoUpdateAllStatuses();
            Order *o = OrderList::findOrder(orderId);
            if (o && o->getUsername() == Customer::getCurrentUser())
            {
                if (o->canModify())
                {
                    o->restoreStock();                                         // 恢复库存
                    OrderList::removeOrder(o->getOrderId(), o->getUsername()); // 移除订单
                    cout << "订单已取消\n";
                }
                else
                    cout << "订单不可取消\n";
            }
            break;
        }
        case 4:
        { // 删除订单
            cout << "输入订单号：";
            cin >> orderId;
            OrderList::autoUpdateAllStatuses();
            if (OrderList::removeOrder(orderId, Customer::getCurrentUser()))
            {
                cout << "删除成功\n";
            }
            else
            {
                cout << "只能删除已签收订单\n";
            }
            break;
        }
        }
    }
}

// 顾客主菜单
void menuCustomer()
{
    int choice;
    string u, p, newp;
    while (true)
    {
        cout << "\n[顾客菜单]\n"
             << "1. 注册\n"
             << "2. 登录\n"
             << "3. 修改密码\n"
             << "4. 删除账号\n"
             << "5. 修改用户名\n"
             << "6. 查询商品并购买\n"
             << "7. 购物车管理与购买\n"
             << "8. 查询订单\n"
             << "0. 返回\n选择：";
        cin >> choice;

        if (choice == 0)
            break; // 返回主菜单

        switch (choice)
        {
        case 1: // 注册
            cout << "用户名：";
            cin >> u;
            cout << "密码：";
            cin >> p;
            if (Customer::registerAccount(u, p))
                cout << "注册成功。\n";
            else
                cout << "用户名已存在。\n";
            break;
        case 2: // 登录
            cout << "用户名：";
            cin >> u;
            cout << "密码：";
            cin >> p;
            if (Customer::login(u, p))
                cout << "登录成功。\n";
            else
                cout << "登录失败。\n";
            break;
        case 3: // 修改密码
            cout << "用户名：";
            cin >> u;
            cout << "旧密码：";
            cin >> p;
            cout << "新密码：";
            cin >> newp;
            if (Customer::changePassword(u, p, newp))
                cout << "修改成功。\n";
            else
                cout << "用户名或密码错误。\n";
            break;
        case 4: // 删除账号
            cout << "用户名：";
            cin >> u;
            cout << "密码：";
            cin >> p;
            if (Customer::deleteAccount(u, p))
                cout << "删除成功。\n";
            else
                cout << "用户名或密码错误。\n";
            break;
        case 5: // 修改用户名
            cout << "旧用户名：";
            cin >> u;
            cout << "密码：";
            cin >> p;
            cout << "新用户名：";
            cin >> newp;
            if (Customer::changeUsername(u, p, newp))
                cout << "用户名修改成功。\n";
            else
                cout << "失败：用户名/密码错误或新用户名已存在。\n";
            break;
        case 6: // 商品查询与购买
            menuQueryProduct();
            break;
        case 7: // 购物车管理
            menuShoppingCart();
            break;
        case 8: // 订单查询
            menuOrderManagement();
            break;
        default:
            cout << "无效选择！\n";
        }
    }
}

// 创建订单功能
void menuCreateOrder()
{
    if (!Customer::isLoggedIn())
    {
        cout << "请先登录！\n";
        return;
    }

    // 从当前购物车获取原始数据
    vector<CartItem> rawItems = ShoppingCart::getCurrentCart();
    vector<pair<string, int>> orderItems; // 订单商品列表
    double total = 0.0;                   // 订单总价

    // 检查库存并转换类型
    for (auto &item : rawItems)
    {
        Product *p = ProductList::findProductByName(item.productName);
        if (!p || p->getStock() < item.quantity)
        {
            cout << "商品 " << item.productName << " 库存不足！\n";
            return;
        }

        // 类型转换
        orderItems.push_back({item.productName, item.quantity});
        total += p->getPrice() * item.quantity;
    }

    string address;
    cout << "输入收货地址：";
    cin.ignore();
    getline(cin, address);

    // 创建订单
    Order newOrder(Customer::getCurrentUser(), orderItems, total, address);
    OrderList::addOrder(newOrder);
    OrderList::autoUpdateAllStatuses();

    // 更新库存
    for (auto &item : rawItems)
    {
        ProductList::updateStock(item.productName, -item.quantity);
    }

    // 清空购物车
    ShoppingCart::clearCart();
    cout << "订单创建成功！\n";
}

// 主函数
int main()
{
    // 初始化数据
    ProductList::loadProducts();         // 加载商品数据
    ShoppingCart::loadCart();            // 加载购物车数据
    OrderList::loadOrders("orders.txt"); // 加载订单数据

    int role;
    string u, p;
    while (true)
    {
        cout << "\n[主菜单]\n1. 管理员登录\n2. 顾客操作\n0. 退出\n选择：";
        cin >> role;
        if (role == 0)
            break; // 退出程序

        if (role == 1)
        { // 管理员登录
            cout << "管理员账户：";
            cin >> u;
            cout << "密码：";
            cin >> p;
            if (Admin::login(u, p))
                menuAdmin(); // 进入管理员菜单
            else
                cout << "登录失败。\n";
        }
        else if (role == 2) // 顾客操作
            menuCustomer(); // 进入顾客菜单
        else
            cout << "无效选择。\n";
    }

    // 程序退出前保存数据
    OrderList::saveOrders("orders.txt");
    return 0;
}
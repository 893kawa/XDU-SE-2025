#include "ProductList.h"
#include "PromotionManager.h"
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

// 静态成员初始化
vector<Product> ProductList::products;

// 从文件加载商品数据
void ProductList::loadProducts()
{
    ifstream ifs("products.txt");
    if (ifs.is_open())
    {
        string cat, name, desc;
        double price;
        int stock;
        // 读取商品数据
        while (ifs >> cat >> name >> price)
        {
            ifs.ignore();
            getline(ifs, desc);
            ifs >> stock;
            products.emplace_back(cat, name, price, desc, stock); // 添加到列表
        }
        ifs.close();
    }
}

// 保存商品数据到文件
void ProductList::saveProducts()
{
    ofstream ofs("products.txt");
    if (ofs.is_open())
    {
        for (const auto &p : products)
        {
            // 写入商品信息
            ofs << p.getCategory() << " " << p.getName() << " " << p.getPrice() << endl;
            ofs << p.getDescription() << endl;
            ofs << p.getStock() << endl;
        }
        ofs.close();
    }
}

// 添加新商品
void ProductList::addProduct(const Product &product)
{
    // 检查商品是否已存在
    for (const auto &p : products)
    {
        if (p.getName() == product.getName())
        {
            cout << "商品已存在，无法重复添加！\n";
            return;
        }
    }
    products.push_back(product); // 添加新商品
    saveProducts();              // 保存更改
    cout << "商品添加成功：" << product.getName() << "，库存：" << product.getStock() << "\n";
}

// 显示所有商品
void ProductList::showAll()
{
    // 显示订单促销信息
    auto orderPromotions = PromotionManager::instance().getOrderPromotions();
    if (!orderPromotions.empty())
    {
        cout << "[订单促销活动]\n";
        for (auto *promo : orderPromotions)
        {
            cout << "  - " << promo->getLabel() << "\n";
        }
    }

    // 检查商品列表是否为空
    if (products.empty())
    {
        cout << "当前无商品！\n";
        return;
    }
    // 遍历并显示所有商品
    for (const auto &p : products)
        cout << p.toString() << endl;
}

// 修改商品信息
bool ProductList::modifyProduct(const string &name)
{
    for (auto &p : products)
    {
        if (p.getName() == name)
        {
            string cat, desc;
            double price;
            int stock;
            // 获取新信息
            cout << "请输入新的类别：";
            cin >> cat;
            cout << "请输入新的价格：";
            cin >> price;
            cout << "请输入新的描述：";
            cin.ignore();
            getline(cin, desc);
            cout << "请输入新的库存：";
            cin >> stock;

            // 更新商品信息
            p.setCategory(cat);
            p.setPrice(price);
            p.setDescription(desc);
            p.setStock(stock);

            saveProducts(); // 保存更改
            cout << "商品信息修改成功！\n";
            return true; // 修改成功
        }
    }
    cout << "未找到该商品！\n";
    return false; // 未找到商品
}

// 删除商品
bool ProductList::deleteProduct(const string &name)
{
    // 查找并删除商品
    auto it = remove_if(products.begin(), products.end(),
                        [&name](const Product &p)
                        { return p.getName() == name; });
    if (it != products.end())
    {
        products.erase(it, products.end()); // 从列表中删除
        saveProducts();                     // 保存更改
        cout << "商品删除成功！\n";
        return true; // 删除成功
    }
    cout << "未找到该商品！\n";
    return false; // 未找到商品
}

// 精确查询商品
void ProductList::searchExact(const string &keyword)
{
    bool found = false; // 标记是否找到匹配
    // 遍历商品列表
    for (const auto &p : products)
    {
        // 检查名称、类别或价格是否完全匹配
        if (p.getName() == keyword || p.getCategory() == keyword ||
            to_string(p.getPrice()).find(keyword) != string::npos)
        {
            cout << p.toString() << endl; // 显示匹配商品
            found = true;
        }
    }
    if (!found)
    {
        cout << "未找到匹配的商品！\n";
    }
}

// 模糊查询商品
void ProductList::searchFuzzy(const string &keyword)
{
    cout << "模糊匹配查询，包含关键词：" << keyword << " 的商品：\n";
    bool found = false; // 标记是否找到匹配
    // 遍历商品列表
    for (const auto &p : products)
    {
        // 检查名称、类别、描述或价格是否包含关键词
        if (p.getName().find(keyword) != string::npos ||
            p.getCategory().find(keyword) != string::npos ||
            p.getDescription().find(keyword) != string::npos ||
            to_string(p.getPrice()).find(keyword) != string::npos)
        {
            cout << p.toString() << endl; // 显示匹配商品
            found = true;
        }
    }
    if (!found)
    {
        cout << "未找到匹配的商品！\n";
    }
}

// 按名称查找商品
Product *ProductList::findProductByName(const string &name)
{
    for (auto &p : products)
    {
        if (p.getName() == name)
        {
            return &p; // 返回商品指针
        }
    }
    return nullptr; // 未找到商品
}

// 更新商品库存
void ProductList::updateStock(const std::string &productName, int change)
{
    for (auto &product : products)
    {
        if (product.getName() == productName)
        {
            int newStock = product.getStock() + change; // 计算新库存
            if (newStock < 0)
            {
                cout << "库存不足，无法更新！\n";
                return;
            }
            product.setStock(newStock); // 更新库存
            saveProducts();             // 保存更改
            cout << "商品库存已更新：" << product.getName() << "，剩余库存：" << product.getStock() << "\n";
            return;
        }
    }
    cout << "商品未找到，无法更新库存！\n";
}
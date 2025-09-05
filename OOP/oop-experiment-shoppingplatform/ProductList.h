#ifndef PRODUCTLIST_H
#define PRODUCTLIST_H

#include "Product.h"
#include <vector>
#include <string>
using namespace std;

// 商品管理类
class ProductList
{
private:
    static vector<Product> products; // 所有商品的静态容器

public:
    // 添加商品
    static void addProduct(const Product &p);

    // 显示所有商品
    static void showAll();

    // 修改商品信息
    static bool modifyProduct(const string &name);

    // 删除商品
    static bool deleteProduct(const string &name);

    // 精确查询
    static void searchExact(const string &keyword);

    // 模糊查询
    static void searchFuzzy(const string &keyword);

    // 按名称查找商品
    static Product *findProductByName(const string &name);

    // 更新库存
    static void updateStock(const std::string &productName, int change);

    // 从文件加载商品
    static void loadProducts();

    // 保存商品到文件
    static void saveProducts();
};

#endif
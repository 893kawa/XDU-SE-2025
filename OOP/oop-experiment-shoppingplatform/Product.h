#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
using namespace std;

// 商品类
class Product
{
private:
    string category;    // 商品类别
    string name;        // 商品名称
    double price;       // 商品价格
    string description; // 商品描述
    int stock;          // 库存数量

public:
    // 构造函数
    Product(string cat, string n, double p, string d, int s);

    // Getter方法
    string getCategory() const;
    string getName() const;
    double getPrice() const;
    string getDescription() const;

    // Setter方法
    void setCategory(const string &cat);
    void setName(const string &n);
    void setPrice(double p);
    void setDescription(const string &d);

    // 转换为字符串表示
    string toString() const;

    // 库存操作
    int getStock() const;
    void setStock(int s);
};

#endif
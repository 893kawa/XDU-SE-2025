#include "Product.h"
#include "PromotionManager.h"
#include <sstream>
#include <iomanip>

// 商品构造函数
Product::Product(string cat, string n, double p, string d, int s)
    : category(cat), name(n), price(p), description(d), stock(s) {}

// Getter方法实现
string Product::getCategory() const { return category; }
string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
string Product::getDescription() const { return description; }

// Setter方法实现
void Product::setCategory(const string &cat) { category = cat; }
void Product::setName(const string &n) { name = n; }
void Product::setPrice(double p) { price = p; }
void Product::setDescription(const string &d) { description = d; }

// 转换为字符串表示
string Product::toString() const
{
    stringstream ss;
    ss << fixed << setprecision(2); // 固定小数点位数
    ss << "[商品] " << name << "\n"
       << "  类别：" << category << "\n"
       << "  价格：" << price << "\n"
       << "  描述：" << description << "\n"
       << "  库存：" << stock << "\n";

    // 添加促销信息
    for (auto *promo : PromotionManager::instance().getProductPromotions(name))
    {
        if (promo->isActive())
            ss << "  [促销] " << promo->getLabel() << "\n";
    }
    return ss.str(); // 返回格式化字符串
}

// 获取库存
int Product::getStock() const { return stock; }

// 设置库存
void Product::setStock(int s) { stock = s; }
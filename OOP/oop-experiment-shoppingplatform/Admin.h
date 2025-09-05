#ifndef ADMIN_H
#define ADMIN_H
#include <string>
using namespace std;

// 管理员类声明
class Admin
{
public:
    // 静态成员函数：管理员登录验证
    static bool login(const string &user, const string &pass);

    // 显示所有顾客信息
    static void showAllCustomers();

    // 重置顾客密码
    static bool resetCustomerPassword(const string &user, const string &newPass);
};

#endif
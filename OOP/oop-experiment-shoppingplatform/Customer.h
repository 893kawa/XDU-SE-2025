#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <string>
using namespace std;

// 顾客类声明
class Customer
{
private:
    static string currentUser; // 静态成员变量：当前登录用户

    // 检查用户名是否存在
    static bool userExists(const string &user);

public:
    // 用户注册
    static bool registerAccount(const string &user, const string &pass);

    // 用户登录
    static bool login(const string &user, const string &pass);

    // 修改密码
    static bool changePassword(const string &user, const string &oldPass, const string &newPass);

    // 删除账户
    static bool deleteAccount(const string &user, const string &pass);

    // 修改用户名
    static bool changeUsername(const string &user, const string &pass, const string &newUser);

    // 用户登出
    static void logout();

    // 检查登录状态
    static bool isLoggedIn();

    // 获取当前用户名
    static string getCurrentUser();
};
#endif
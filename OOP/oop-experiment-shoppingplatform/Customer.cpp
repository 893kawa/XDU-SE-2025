#include "Customer.h"
#include <fstream>
#include <vector>
using namespace std;

// 检查用户是否存在
bool Customer::userExists(const string &user)
{
    ifstream ifs("users.txt");
    string u, p;
    // 遍历用户文件查找匹配的用户名
    while (ifs >> u >> p)
        if (u == user)
            return true; // 找到用户
    return false;        // 未找到用户
}

// 注册新账户
bool Customer::registerAccount(const string &user, const string &pass)
{
    // 检查用户名是否已存在
    if (userExists(user))
        return false;

    // 以追加模式打开文件写入新用户
    ofstream ofs("users.txt", ios::app);
    ofs << user << " " << pass << endl;
    return true; // 注册成功
}

// 顾客登录验证
bool Customer::login(const string &user, const string &pass)
{
    ifstream ifs("users.txt");
    string u, p;
    // 遍历用户文件验证凭据
    while (ifs >> u >> p)
        if (u == user && p == pass)
        {
            currentUser = user; // 设置当前登录用户
            ifs.close();
            return true; // 登录成功
        }
    ifs.close();
    return false; // 登录失败
}

// 修改密码
bool Customer::changePassword(const string &user, const string &oldPass, const string &newPass)
{
    ifstream ifs("users.txt");
    vector<pair<string, string>> users; // 存储所有用户数据
    string u, p;
    bool found = false; // 标记是否找到匹配的用户

    // 遍历用户文件
    while (ifs >> u >> p)
    {
        if (u == user && p == oldPass)
        {
            // 找到匹配用户，更新密码
            users.push_back({u, newPass});
            found = true;
        }
        else
        {
            // 其他用户保持不变
            users.push_back({u, p});
        }
    }
    ifs.close();
    if (!found)
        return false; // 未找到匹配用户

    // 写入更新后的用户数据
    ofstream ofs("users.txt");
    for (auto &up : users)
        ofs << up.first << " " << up.second << endl;
    return true; // 修改成功
}

// 删除账户
bool Customer::deleteAccount(const string &user, const string &pass)
{
    ifstream ifs("users.txt");
    vector<pair<string, string>> users; // 存储保留的用户
    string u, p;
    bool found = false; // 标记是否找到匹配用户

    // 遍历用户文件
    while (ifs >> u >> p)
    {
        if (u == user && p == pass)
        {
            found = true; // 找到匹配用户，跳过写入
            continue;
        }
        users.push_back({u, p}); // 保留其他用户
    }
    ifs.close();
    if (!found)
        return false; // 未找到匹配用户

    // 写入保留的用户数据（删除指定用户）
    ofstream ofs("users.txt");
    for (auto &up : users)
        ofs << up.first << " " << up.second << endl;
    return true; // 删除成功
}

// 修改用户名
bool Customer::changeUsername(const string &user, const string &pass, const string &newUser)
{
    // 检查新用户名是否已存在
    if (userExists(newUser))
        return false;

    ifstream ifs("users.txt");
    vector<pair<string, string>> users; // 存储更新后的用户数据
    string u, p;
    bool found = false; // 标记是否找到匹配用户

    // 遍历用户文件
    while (ifs >> u >> p)
    {
        if (u == user && p == pass)
        {
            // 找到匹配用户，更新用户名
            users.push_back({newUser, p});
            found = true;
        }
        else
        {
            // 其他用户保持不变
            users.push_back({u, p});
        }
    }
    ifs.close();
    if (!found)
        return false; // 未找到匹配用户

    // 写入更新后的用户数据
    ofstream ofs("users.txt");
    for (auto &up : users)
        ofs << up.first << " " << up.second << endl;
    return true; // 修改成功
}

// 静态成员变量初始化
string Customer::currentUser = "";

// 注销当前用户
void Customer::logout() { currentUser.clear(); }

// 检查用户是否登录
bool Customer::isLoggedIn() { return !currentUser.empty(); }

// 获取当前登录用户名
string Customer::getCurrentUser() { return currentUser; }
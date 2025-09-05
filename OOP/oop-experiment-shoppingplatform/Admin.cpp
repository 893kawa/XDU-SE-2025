#include "Admin.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// 管理员登录验证
bool Admin::login(const string &user, const string &pass)
{
    // 检查用户名和密码是否匹配预设的管理员凭证
    return user == "admin" && pass == "admin123";
}

// 显示所有顾客信息
void Admin::showAllCustomers()
{
    // 打开存储用户信息的文件
    ifstream ifs("users.txt");
    string u, p;
    cout << "\n[所有顾客列表]\n";
    // 逐行读取文件内容并输出用户名
    while (ifs >> u >> p)
        cout << "- " << u << endl;
    ifs.close(); // 关闭文件流
}

// 重置顾客密码
bool Admin::resetCustomerPassword(const string &user, const string &newPass)
{
    // 打开用户文件
    ifstream ifs("users.txt");
    vector<pair<string, string>> users; // 存储所有用户数据
    string u, p;
    bool found = false; // 标记是否找到目标用户

    // 遍历所有用户
    while (ifs >> u >> p)
    {
        if (u == user)
        {
            // 找到目标用户，更新密码
            users.push_back({u, newPass});
            found = true;
        }
        else
        {
            // 其他用户保持原密码
            users.push_back({u, p});
        }
    }
    ifs.close(); // 关闭输入流

    // 未找到用户返回false
    if (!found)
        return false;

    // 打开输出流写入更新后的用户数据
    ofstream ofs("users.txt");
    for (auto &up : users)
        ofs << up.first << " " << up.second << endl;
    ofs.close(); // 关闭输出流
    return true; // 操作成功
}
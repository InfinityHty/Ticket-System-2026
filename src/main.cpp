//
// Created by HTY on 2026/5/25.
//
#include "../include/User.h"
#include "../include/Train.h"
#include "../include/vector.h"
#include "../include/Account.h"
#include "../include/Convert_functions.h"
#include<iostream>
#include<string>
#include<cstdio>
#include<cmath>
#include<fstream>
#include<filesystem>
sjtu::vector<User> login;
char record[20];
int main() {
    Account ac;
    ac.Init();
    while (true) {
        bool valid = true;
        std::string input;
        getline(std::cin,input);
        std::stringstream instruction(input);
        std::string timestamp,cmd;
        instruction >> timestamp;
        instruction >> cmd;
        if (cmd == "exit") {
            ac.Close();
            std::cout << timestamp << " bye";
            break;
        }
        if (cmd == "clean") {
            // 删除所有数据
        }
        // 用户账户操作
        if (cmd == "add_user") {
            std::string c,u,p,n,m;
            std::string prev,tmp;
            int g;
            while (instruction >> tmp) {
                if (tmp == "-c" || tmp == "-u" || tmp == "-p" || tmp == "-n" || tmp == "-m" || tmp == "-g") {
                    prev = tmp;
                    continue;
                }
                else {
                    if (prev == "-c") c = tmp;
                    if (prev == "-u") u = tmp;
                    if (prev == "-p") p = tmp;
                    if (prev == "-n") n = tmp;
                    if (prev == "-m") m = tmp;
                    if (prev == "-g") g = StringToInt(tmp);
                }
            }
            // std::cerr << u << " " << p << " " << n << " " << m << " " << g << '\n';
            User new_user(u,p,n,m,g);
            if (!ac.HasAnyUser()) {
                // 创建第一个用户
                new_user.privilege = 10;
                ac.AddUser(new_user);
                std::cout << timestamp << " " << 0 << '\n';
            }
            else {
                // 检查合法性
                if (ac.ExistThisUser(new_user.username)) valid = false;
                else {
                    bool has_login = false;
                    User cur;
                    size_t num = login.size();
                    for (int i = 0; i < num; i++) {
                        if (login[i].username == c) {
                            has_login = true;
                            cur = login[i];
                        }
                    }
                    if (!has_login || cur.privilege <= g) valid = false;
                    else {
                        ac.AddUser(new_user);
                        std::cout << timestamp << " " << 0 << '\n';
                    }
                }
            }
        }
        if (cmd == "login") {
            std::string u,p;
            std::string tmp,prev;
            while (instruction >> tmp) {
                if (tmp == "-u" || tmp == "-p") {
                    prev = tmp;
                    continue;
                }
                if (prev == "-u") u = tmp;
                if (prev == "-p") p = tmp;
            }
            char tmp_u[20];
            StringToChar20(tmp_u,u);
            if (!ac.ExistThisUser(tmp_u)) valid = false;
            else {
                User cur = ac.GetUser(tmp_u);
                if (cur.password != p) valid = false;
                else {
                    size_t num = login.size();
                    for (int i = 0; i < num; i++) {
                        if (login[i] == cur) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid) {
                        login.push_back(cur);
                        std::cout << timestamp << " " << 0 << '\n';
                    }
                }
            }
        }
        if (cmd == "logout") {
            std::string u,tmp;
            instruction >> tmp;
            if (tmp == "-u") instruction >> u;
            bool exist = false;
            for (auto it = login.begin(); it != login.end(); it++) {
                User tmp_u = *it;
                if (tmp_u.username == u) {
                    exist = true;
                    login.erase(it);
                    break;
                }
            }
            if (!exist) {
                valid = false;
            }
            else {
                std::cout << timestamp << " " << 0 << '\n';
            }
        }
        if (cmd == "query_profile") {
            std::string c,u,tmp,prev;
            while (instruction >> tmp) {
                if (tmp == "-c" || tmp == "-u") {
                    prev = tmp;
                    continue;
                }
                if (prev == "-c") c = tmp;
                if (prev == "-u") u = tmp;
            }
            char tmp_username[20];
            StringToChar20(tmp_username,u);
            User cur;
            bool exist = false;
            for (auto tmp_u : login) {
                if (tmp_u.username == c) {
                    exist = true;
                    cur = tmp_u;
                    break;
                }
            }
            if (!exist || !ac.ExistThisUser(tmp_username)) valid = false;
            else {
                User query = ac.GetUser(tmp_username);
                if (cur.privilege < query.privilege) valid = false;
                else {
                    std::cout << timestamp << " " << query << '\n';
                }
            }
        }
        if (cmd == "modify_profile") {
            std::string c,u,p,n,m;
            int g = -1;
            std::string tmp,prev;
            while (instruction >> tmp) {
                if (tmp == "-c" || tmp == "-u" || tmp == "-p" || tmp == "-n" || tmp == "-m" || tmp == "-g") {
                    prev = tmp;
                    continue;
                }
                if (prev == "-c") c = tmp;
                if (prev == "-u") u = tmp;
                if (prev == "-p") p = tmp;
                if (prev == "-n") n = tmp;
                if (prev == "-m") m = tmp;
                if (prev == "-g") g = StringToInt(tmp);
            }
            User cur;
            bool exist = false;
            for (auto tmp_u : login) {
                if (tmp_u.username == c) {
                    cur = tmp_u;
                    exist = true;
                    break;
                }
            }
            if (!exist) valid = false;
            else {
                char tmp_username[20];
                StringToChar20(tmp_username,u);
                if (!ac.ExistThisUser(tmp_username)) valid = false;
                else {
                    User old = ac.GetUser(tmp_username);
                    if (cur.privilege < old.privilege || g >= cur.privilege) valid = false;
                    else {
                        ac.DeleteUser(old);
                        if (p.empty()) p = old.password;
                        if (n.empty()) n = old.name;
                        if (m.empty()) m = old.mailAddr;
                        if (g == -1) g = old.privilege;
                        User modify(u,p,n,m,g);
                        ac.AddUser(modify);
                        std::cout << timestamp << " " << modify << '\n';
                    }
                }
            }
        }
        // ===

        // 火车信息模块

        // ===
        if (!valid) {
            std::cout << timestamp << " "  << -1 << "\n";
        }
    }
    return 0;
}
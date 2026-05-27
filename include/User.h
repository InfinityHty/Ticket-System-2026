//
// Created by HTY on 2026/5/25.
//

#ifndef TICKET_SYSTEM_2026_USER_H
#define TICKET_SYSTEM_2026_USER_H
#include<string>
#include<iostream>
class User {
public:
    char username[20];
    char password[30];
    char name[20];
    char mailAddr[30];
    int privilege;
    User() {
        for (int i = 0; i < 20; i++) {
            username[i] = '\0';
            name[i] = '\0';
            password[i] = '\0';
            mailAddr[i] = '\0';
        }
        for (int i = 20; i < 30; i++) {
            password[i] = mailAddr[i] = '\0';
        }
        privilege = 10;
    }
    User(std::string &username_, std::string &password_, std::string &name_, std::string &mailAddr_, int privilege_) {
        int size_ = username_.size();
        for (int i = 0; i < size_; i++) username[i] = username_[i];
        for (int i = size_; i < 20; i++) username[i] = '\0';
        size_ = password_.size();
        for (int i = 0; i < size_; i++) password[i] = password_[i];
        for (int i = size_; i < 30; i++) password[i] = '\0';
        size_ = name_.size();
        for (int i = 0; i < size_; i++) name[i] = name_[i];
        for (int i = size_; i < 20; i++) name[i] = '\0';
        size_ = mailAddr_.size();
        for (int i = 0; i < size_; i++) mailAddr[i] = mailAddr_[i];
        for (int i = size_; i < 30; i++) mailAddr[i] = '\0';
        privilege = privilege_;
        // std::cerr << username << " " << password << " " << name << " " << mailAddr << " " << privilege << '\n';
    }
    friend bool operator <(User &a,User &b) {
        for (int i = 0; i < 20; i++) {
            if (a.username[i] < b.username[i]) return true;
            if (a.username[i] > b.username[i]) return false;
        }
        return false;
    }
    friend bool operator >(User &a,User &b) {
        for (int i = 0; i < 20; i++) {
            if (a.username[i] > b.username[i]) return true;
            if (a.username[i] < b.username[i]) return false;
        }
        return false;
    }
    friend bool operator ==(User &a,User &b) {
        for (int i = 0; i < 20; i++) {
            if (a.username[i] != b.username[i]) return false;
        }
        return true;
    }
    friend bool operator <=(User &a,User &b) {
        for (int i = 0; i < 20; i++) {
            if (a.username[i] < b.username[i]) return true;
            if (a.username[i] > b.username[i]) return false;
        }
        return true;
    }
    friend bool operator >=(User &a,User &b) {
        for (int i = 0; i < 20; i++) {
            if (a.username[i] > b.username[i]) return true;
            if (a.username[i] < b.username[i]) return false;
        }
        return true;
    }
    friend bool operator !=(User &a,User &b) {
        for (int i = 0; i < 20; i++) {
            if (a.username[i] != b.username[i]) return true;
        }
        return false;
    }
    friend std::ostream& operator <<(std::ostream &os,User &user) {
        os << user.username << " " << user.name << " " << user.mailAddr << " " << user.privilege;
        return os;
    }
};
#endif //TICKET_SYSTEM_2026_USER_H

// 账户数据涉及以下内容：
// username：用户的账户名，作为用户的唯一标识符，为由字母开头，由字母、数字和下划线组成的字符串，长度不超过 20。
// password：由可见字符组成的字符串，长度不低于 1，不超过 30。
// name：用户真实姓名，由 2 至 5 个汉字组成的字符串。
// mailAddr：同一般邮箱地址格式，且仅含数字、大小写字母，@ 和 .，长度不超过 30（无需检验邮箱格式的合法性）。
// privilege：所处用户组优先级，为 0~10 中的一个整数。
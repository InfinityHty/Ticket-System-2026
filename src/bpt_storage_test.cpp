//
// Created by HTY on 2026/4/26.
//
#include<iostream>
#include<string>
#include "../include/bpt_Database.h"
int n;
// 定长的字符数组类
const int MAX_LEN = 64;
struct Key {
    char buffer[MAX_LEN];
    Key() {
        for (int i = 0; i < MAX_LEN; i++) buffer[i] = '\0';
    }
    Key(const std::string &str) {
        int len = str.size();
        for (int i = 0; i < len; i++) buffer[i] = str[i];
        for (int i = len; i < MAX_LEN; i++) buffer[i] = '\0';
    }
    bool operator==(const Key &other) const {
        for (int i = 0; i < MAX_LEN; i++) {
            if (buffer[i] != other.buffer[i]) return false;
        }
        return true;
    }
    bool operator < (const Key &other) const {
        for (int i = 0; i < MAX_LEN; i++) {
            if (buffer[i] < other.buffer[i]) return true;
            else if (buffer[i] > other.buffer[i]) return false;
        }
        return false;
    }
    bool operator <= (const Key &other) const {
        for (int i = 0; i < MAX_LEN; i++) {
            if (buffer[i] < other.buffer[i]) return true;
            else if (buffer[i] > other.buffer[i]) return false;
        }
        return true;
    }
    bool operator > (const Key &other) const {
        for (int i = 0; i < MAX_LEN; i++) {
            if (buffer[i] > other.buffer[i]) return true;
            else if (buffer[i] < other.buffer[i]) return false;
        }
        return false;
    }
    bool operator >= (const Key &other) const {
        for (int i = 0; i < MAX_LEN; i++) {
            if (buffer[i] > other.buffer[i]) return true;
            else if (buffer[i] < other.buffer[i]) return false;
        }
        return true;
    }
    friend std::ostream& operator << (std::ostream &os,Key &str) {
        int id = 0;
        while (str.buffer[id] != '\0') {
            os << str.buffer[id++];
        }
        return os;
    }
};
Database<Key,int> db;
int main(){
    std::cin >> n;
    db.Initialize("index.txt","data.txt");
    for (int i = 0; i < n; i++) {
        std::string op,name;
        int value;
        std::cin >> op;
        if (op == "insert") {
            std::cin >> name >> value;
            std::cout << db.HasData() << " " << db.Exist(Key(name)) << '\n';
            db.Insert(Key(name),value);
        }
        if (op == "delete") {
            std::cin >> name >> value;
            std::cout << db.HasData() << " " << db.Exist(Key(name)) << '\n';
            db.Delete(Key(name),value);
        }
        if (op == "find") {
            std::cin >> name;
            std::cout << db.HasData() << " " << db.Exist(Key(name)) << '\n';
            db.Find(Key(name));
        }
    }
    db.Close();
    return 0;
}
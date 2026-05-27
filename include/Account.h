//
// Created by HTY on 2026/5/25.
//

#ifndef TICKET_SYSTEM_2026_ACCOUNT_H
#define TICKET_SYSTEM_2026_ACCOUNT_H
#include "User.h"
#include "bpt_Database.h"
#include<string>
class Account {
public:
    std::string index_file_name = "account_index.txt";
    std::string file_name = "account.txt";
    struct Key {
        char cont[20];
        Key() {
            for (int i = 0; i < 20; i++) cont[i] = '\0';
        }
        Key(const char *other) {
            for (int i = 0; i < 20; i++) cont[i] = '\0';
            for (int i = 0; i < 20; i++) cont[i] = other[i];
        }
        friend bool operator <(Key &a,Key &b) {
            for (int i = 0; i < 20; i++) {
                if (a.cont[i] < b.cont[i]) return true;
                if (a.cont[i] > b.cont[i]) return false;
            }
            return false;
        }
        friend bool operator >(Key &a,Key &b) {
            for (int i = 0; i < 20; i++) {
                if (a.cont[i] > b.cont[i]) return true;
                if (a.cont[i] < b.cont[i]) return false;
            }
            return false;
        }
        friend bool operator ==(Key &a,Key &b) {
            for (int i = 0; i < 20; i++) {
                if (a.cont[i] != b.cont[i]) return false;
            }
            return true;
        }
        friend bool operator <=(Key &a,Key &b) {
            for (int i = 0; i < 20; i++) {
                if (a.cont[i] < b.cont[i]) return true;
                if (a.cont[i] > b.cont[i]) return false;
            }
            return true;
        }
        friend bool operator >=(Key &a,Key &b) {
            for (int i = 0; i < 20; i++) {
                if (a.cont[i] > b.cont[i]) return true;
                if (a.cont[i] < b.cont[i]) return false;
            }
            return true;
        }
        friend bool operator !=(Key &a,Key &b) {
            for (int i = 0; i < 20; i++) {
                if (a.cont[i] != b.cont[i]) return true;
            }
            return false;
        }
        friend std::ostream& operator <<(std::ostream &os,Key &k){
            os << k.cont;
            return os;
        }
    };
    // Key是定长为20位的字符串
    Database<Key,User,50,50> db;
    void Init() {
        db.Initialize(index_file_name,file_name);
    }
    void Close() {
        db.Close();
    }
    void Find(const char *username_) {
        db.Find(Key(username_));
    }
    bool HasAnyUser() {
        return db.HasData();
    }
    bool ExistThisUser(const char *username_) {
        return db.Exist(Key(username_));
    }
    User GetUser(Key username) {
        return db.GetData(username);
    }
    void AddUser(User &user) {
        db.Insert(Key(user.username),user);
    }
    void DeleteUser(User &user) {
        db.Delete(Key(user.username),user);
    }
};
#endif //TICKET_SYSTEM_2026_ACCOUNT_H
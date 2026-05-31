//
// Created by HTY on 2026/5/25.
//

#ifndef TICKET_SYSTEM_2026_ACCOUNT_H
#define TICKET_SYSTEM_2026_ACCOUNT_H
#include "User.h"
#include "BPT_Database.h"
#include "Keys.h"
#include "Order.h"
#include "Tool_functions.h"
#include<string>
class Account {
public:
    std::string index_file_name = "account_index.txt";
    std::string file_name = "account.txt";
    Database<Key20,User,50,50> account_db;// username->User所有信息
    Database<Key20,Order> order_db;// username->这个user的所有订单
    void Init() {
        account_db.Initialize(index_file_name,file_name);
        order_db.Initialize("order_index.txt","order.txt");
    }
    void Close() {
        account_db.Close();
        order_db.Close();
    }
    void Clear() {
        account_db.Clear();
        order_db.Clear();
    }
    void Find(const char *username_) {
        account_db.Find(Key20(username_));
    }
    bool HasAnyUser() {
        return account_db.HasData();
    }
    bool ExistThisUser(const char *username_) {
        return account_db.Exist(Key20(username_));
    }
    User GetUser(const char *username_) {
        return account_db.GetData(Key20(username_));
    }
    void AddUser(User &user) {
        account_db.Insert(Key20(user.username),user);
    }
    void DeleteUser(User &user) {
        account_db.Delete(Key20(user.username),user);
    }
    void AddUserOrder(Order &order) {
        order_db.Insert(Key20(order.username),order);
    }
    void DeleteUserOrder(Order &order) {
        order_db.Delete(Key20(order.username),order);
    }
    sjtu::vector<Order> GetAllUserOrders(const char *username_) {
        return order_db.GetAllDatas(Key20(username_));
    }
};
#endif //TICKET_SYSTEM_2026_ACCOUNT_H
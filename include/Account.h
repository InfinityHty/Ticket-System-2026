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
#include "MemoryRiver.h"
#include<string>
class Account {
public:
    std::string index_file_name = "account_index.txt";
    std::string file_name = "account.txt";
    Database<Key20,long long,50,50> account_db;// username->User所有信息
    Database<Key20,Order,50,50> order_db;// username->这个user的所有订单
    //Database<Key20,Order,50,50> train_order_db;// trainID->这个train的所有pending订单
    MemoryRiver<User> user_mr;
    // MemoryRiver<Order> order_mr;
    void Init() {
        account_db.Initialize(index_file_name,file_name);
        order_db.Initialize("order_index.txt","order.txt");
        user_mr.Initialize("user_memory.txt");
        //train_order_db.Initialize("train_order_index.txt","train_order.txt");
        // order_mr.Initialize("order_memory.txt");
    }
    void Close() {
        account_db.Close();
        order_db.Close();
        user_mr.Close();
        //train_order_db.Close();
        // order_mr.Close();
    }
    void Clear() {
        account_db.Clear();
        order_db.Clear();
        user_mr.Clear();
        //train_order_db.Clear();
        // order_mr.Clear();
    }
    // void Find(const char *username_) {
    //     account_db.Find(Key20(username_));
    // }
    bool HasAnyUser() {
        return account_db.HasData();
    }
    bool ExistThisUser(const char *username_) {
        return account_db.Exist(Key20(username_));
    }
    User GetUser(const char *username_) {
        User cur;
        user_mr.read(cur,account_db.GetData(Key20(username_)));
        return cur;
    }
    void AddUser(User &user) {
        account_db.Insert(Key20(user.username),user_mr.write(user));
    }
    void DeleteUser(User &user) {
        long long idx = account_db.GetData(user.username);
        account_db.Delete(Key20(user.username),idx);
    }
    void AddUserOrder(Order &order) {
        order_db.Insert(Key20(order.username),order);
    }
    void DeleteUserOrder(Order &order) {
        order_db.Delete(Key20(order.username),order);
    }
    bool ExistThisUserOrder(const char *username_) {
        return order_db.Exist(Key20(username_));
    }
    sjtu::vector<Order> GetAllUserOrders(const char *username_) {
        return order_db.GetAllDatas(username_);
    }


    // void AddTrainOrder(Order &order) {
    //     train_order_db.Insert(Key20(order.ticket.trainID),order);
    // }
    // void DeleteTrainOrder(Order &order) {
    //     train_order_db.Delete(Key20(order.ticket.trainID),order);
    // }
    // bool ExistThisTrainOrder(const Order &order) {
    //     return train_order_db.Exist(order.ticket.trainID);
    // }
    // sjtu::vector<Order> GetAllTrainOrders(const Key20 &trainID_) {
    //     return train_order_db.GetAllDatas(trainID_);
    //     // return sjtu::vector<Order>{};
    // }


};
#endif //TICKET_SYSTEM_2026_ACCOUNT_H
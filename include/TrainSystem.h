//
// Created by HTY on 2026/5/28.
//

#ifndef TICKET_SYSTEM_2026_TRAINSYSTEM_H
#define TICKET_SYSTEM_2026_TRAINSYSTEM_H
#include "Train.h"
#include "BPT_Database.h"
#include "MemoryRiver.h"
#include <string>
#include "Keys.h"
#include "Order.h"
class TrainSystem {
public:
    std::string index_file_name1 = "train_index.txt";
    std::string file_name1 = "train.txt";
    Database<Key20,long long,100,100> train_db;// trainID->Train的所有信息
    Database<Key20,Order,50,50> train_order_db;// trainID->这个train的所有pending订单
    MemoryRiver<Train> train_mr;
    void Init() {
        train_db.Initialize(index_file_name1,file_name1);
        train_order_db.Initialize("train_order_index.txt","train_order.txt");
        train_mr.Initialize("train_memory.txt");
    }
    void Close() {
        train_db.Close();
        train_order_db.Close();
        train_mr.Close();
    }
    void Clear() {
        train_db.Clear();
        train_order_db.Clear();
        train_mr.Clear();
    }
    bool ExistThisTrain(const char *train) {
        return train_db.Exist(Key20(train));
    }
    Train GetTrain(const char *train) {
        Train cur;
        train_mr.read(cur,train_db.GetData(Key20(train)));
        return cur;
    }
    void AddTrain(Train &train) {
        //std::cerr << train.trainID << '\n';
        train_db.Insert(Key20(train.trainID),train_mr.write(train));
    }
    void DeleteTrain(Train &train) {
        long long idx = train_db.GetData(train.trainID);
        train_db.Delete(Key20(train.trainID),idx);
    }
    void AddTrainOrder(Order &order) {
        train_order_db.Insert(Key20(order.ticket.trainID),order);
    }
    void DeleteTrainOrder(Order &order) {
        train_order_db.Delete(Key20(order.ticket.trainID),order);
    }
    sjtu::vector<Order> GetAllTrainOrders(const char *train) {
        return train_order_db.GetAllDatas(Key20(train));
    }
};
#endif //TICKET_SYSTEM_2026_TRAINSYSTEM_H
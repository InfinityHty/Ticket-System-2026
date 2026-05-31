//
// Created by HTY on 2026/5/28.
//

#ifndef TICKET_SYSTEM_2026_TRAINSYSTEM_H
#define TICKET_SYSTEM_2026_TRAINSYSTEM_H
#include "Train.h"
#include "BPT_Database.h"
#include <string>
#include "Keys.h"
#include "Order.h"
class TrainSystem {
public:
    std::string index_file_name1 = "train_index.txt";
    std::string file_name1 = "train.txt";
    Database<Key20,Train,30,30> train_db;// trainID->Train的所有信息
    Database<Key20,Order,50,50> train_order_db;// trainID->这个train的所有pending订单
    void Init() {
        train_db.Initialize(index_file_name1,file_name1);
        train_order_db.Initialize("train_order_index.txt","train_order.txt");
    }
    void Close() {
        train_db.Close();
        train_order_db.Close();
    }
    void Clear() {
        train_db.Clear();
        train_order_db.Clear();
    }
    bool ExistThisTrain(const char *train) {
        return train_db.Exist(Key20(train));
    }
    Train GetTrain(const char *train) {
        return train_db.GetData(Key20(train));
    }
    void AddTrain(Train &train) {
        //std::cerr << train.trainID << '\n';
        train_db.Insert(Key20(train.trainID),train);
    }
    void DeleteTrain(Train &train) {
        train_db.Delete(Key20(train.trainID),train);
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
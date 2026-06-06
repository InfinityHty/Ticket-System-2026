//
// Created by HTY on 2026/5/28.
//

#ifndef TICKET_SYSTEM_2026_TICKETMANAGEMENT_H
#define TICKET_SYSTEM_2026_TICKETMANAGEMENT_H
#include "Ticket.h"
#include "Keys.h"
#include "BPT_Database.h"
#include "vector.h"
#include "Tool_functions.h"
class TicketManagement {
public:
    Database<long long,Key20,30,30> ticket_db;// 起终点信息->trainID
    Database<long long,Key20,30,30> station_db;// 经过某一站->trainID
    // MemoryRiver<TicketKey> ticket_key_mr;
    void Init() {
        ticket_db.Initialize("ticket_index.txt","ticket.txt");
        station_db.Initialize("station_index.txt","station.txt");
        //ticket_key_mr.Initialize("ticket_key_memory.txt");
    }
    void Close() {
        ticket_db.Close();
        station_db.Close();
        //ticket_key_mr.Close();
    }
    void Clear() {
        ticket_db.Clear();
        station_db.Clear();
        //ticket_key_mr.Clear();
    }
    void AddTicket(const long long key, const char *train_id) {
        ticket_db.Insert(key,Key20(train_id));
    }
    void DeleteTicket(const long long key,const char *train_id) {
        ticket_db.Delete(key,Key20(train_id));
    }
    bool ExistTicket(const long long key) {
        return ticket_db.Exist(key);
    }
    void AddTrainToStation(const long long station, const char *train_id) {
        station_db.Insert(station,Key20(train_id));
    }
    bool ExistStation(const long long station) {
        return station_db.Exist(station);
    }
    sjtu::vector<Key20> GetQualifiedTrains(const std::string &st,const std::string &ed) {
        long long idx = HashTicketKey(TicketKey(st,ed));
        return ticket_db.GetAllDatas(idx);
    }
    sjtu::vector<Key20> GetTrainsFromHere(long long station) {
        return station_db.GetAllDatas(station);
    }
    // 给符合要求的车票排序
    void SortByTime(sjtu::vector<Ticket> &t) {
        if (t.empty()) return;
        int num = t.size();
        for (int i = 0; i < num; i++) {
            bool flag = false;
            for (int j = 0; j < num - i - 1; j++) {
                if (t[j].duration > t[j + 1].duration || (t[j].duration == t[j + 1].duration && std::string(t[j].trainID) > std::string(t[j + 1].trainID))) {
                    std::swap(t[j],t[j + 1]);
                    flag = true;
                }
            }
            if (!flag) break;
        }
    }
    void SortByPrice(sjtu::vector<Ticket> &t) {
        if (t.empty()) return;
        int num = t.size();
        for (int i = 0; i < num; i++) {
            bool flag = false;
            for (int j = 0; j < num - i - 1; j++) {
                if (t[j].price > t[j + 1].price || (t[j].price == t[j + 1].price && std::string(t[j].trainID) > std::string(t[j + 1].trainID))) {
                    std::swap(t[j],t[j + 1]);
                    flag = true;
                }
            }
            if (!flag) break;
        }
    }

};
#endif //TICKET_SYSTEM_2026_TICKETMANAGEMENT_H
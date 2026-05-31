//
// Created by HTY on 2026/5/28.
//

#ifndef TICKET_SYSTEM_2026_TICKETMANAGEMENT_H
#define TICKET_SYSTEM_2026_TICKETMANAGEMENT_H
#include "Ticket.h"
#include "Keys.h"
#include "BPT_Database.h"
#include "vector.h"
class TicketManagement {
public:
    void Init() {
        ticket_db.Initialize("ticket_index.txt","ticket.txt");
        station_db.Initialize("station_index.txt","station.txt");
    }
    void Close() {
        ticket_db.Close();
        station_db.Close();
    }
    void Clear() {
        ticket_db.Clear();
        station_db.Clear();
    }
    void AddTicket(const TicketKey &key, const char *train_id) {
        ticket_db.Insert(key,Key20(train_id));
    }
    void AddTrainToStation(const char *station, const char *train_id) {
        station_db.Insert(Key40(station),Key20(train_id));
    }
    sjtu::vector<Key20> GetQualifiedTrains(const std::string &st,const std::string &ed) {
        return ticket_db.GetAllDatas(TicketKey(st,ed));
    }
    sjtu::vector<Key20> GetTrainsFromHere(const char *station) {
        return station_db.GetAllDatas(Key40(station));
    }
    // 给符合要求的车票排序
    void SortByTime(sjtu::vector<Ticket> &t) {
        int num = t.size();
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < num - i - 1; j++) {
                bool flag = false;
                if (t[j].duration > t[j + 1].duration || t[j].duration == t[j + 1].duration && std::string(t[j].trainID) > std::string(t[j + 1].trainID)) {
                    std::swap(t[j],t[j + 1]);
                    flag = true;
                }
                if (!flag) break;
            }
        }
    }
    void SortByPrice(sjtu::vector<Ticket> &t) {
        int num = t.size();
        for (int i = 0; i < num; i++) {
            for (int j = 0; j < num - i - 1; j++) {
                bool flag = false;
                if (t[j].price > t[j + 1].price || t[j].price == t[j + 1].price && std::string(t[j].trainID) > std::string(t[j + 1].trainID)) {
                    std::swap(t[j],t[j + 1]);
                    flag = true;
                }
                if (!flag) break;
            }
        }
    }
private:
    Database<TicketKey,Key20,50,50> ticket_db;// 起终点信息->trainID
    Database<Key40,Key20,50,50> station_db;// 经过某一站->trainID
};
#endif //TICKET_SYSTEM_2026_TICKETMANAGEMENT_H
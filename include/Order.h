//
// Created by HTY on 2026/5/30.
//

#ifndef TICKET_SYSTEM_2026_ORDER_H
#define TICKET_SYSTEM_2026_ORDER_H
#include "Ticket.h"
#include<string>
#include<iostream>
class Order {
public:
    char username[20];
    int timestamp;
    enum Status{success,pending,refunded};
    Status status;
    Ticket ticket;
    int number;
    Order(){}
    Order(const char *username_,int timestamp_,Status status_,Ticket &ticket_,int num) {
        for (int i = 0; i < 20; i++) username[i] = username_[i];
        timestamp = timestamp_;
        status = status_;
        ticket = ticket_;
        number = num;
    }
    friend std::ostream& operator <<(std::ostream& os,Order &o) {
        if (o.status == success) os << "[success] ";
        if (o.status == pending) os << "[pending] ";
        if (o.status == refunded) os << "[refunded] ";
        os << o.ticket.trainID << " " << o.ticket.start << " "  << o.ticket.date[0] << " " << o.ticket.time[0] << " -> "
        << o.ticket.end << " " << o.ticket.date[1] << " "  << o.ticket.time[1] << " " << o.ticket.price << " " << o.number;
        return os;
    }
    bool operator <(const Order &other) {
        if (std::string(username) < other.username) return true;
        if (std::string(username) > other.username) return false;
        if (timestamp < other.timestamp) return true;
        return false;
    }
    bool operator <=(const Order &other) {
        if (std::string(username) < other.username) return true;
        if (std::string(username) > other.username) return false;
        if (timestamp <= other.timestamp) return true;
        return false;
    }
    bool operator ==(const Order &other) {
        if (std::string(username) == other.username && timestamp == other.timestamp) return true;
        return false;
    }
};
#endif //TICKET_SYSTEM_2026_ORDER_H
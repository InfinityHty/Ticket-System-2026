//
// Created by HTY on 2026/5/28.
//

#ifndef TICKET_SYSTEM_2026_TICKET_H
#define TICKET_SYSTEM_2026_TICKET_H
#include "Keys.h"
#include "Train.h"
#include<iostream>
#include<string>
class Ticket {
public:
    char trainID[20];
    char start[40];
    char end[40];
    Train::Date date[2];
    Train::Time time[2];
    int duration;
    int price;
    int seat;
    Ticket(){}
    Ticket(const char *train_id, const char *st_, const char *ed_, Train::Date &d0,
        Train::Time &t0, Train::Date &d1, Train::Time &t1, int dur, int price_,int seat_) {
        for (int i = 0; i < 20; i++) trainID[i] = train_id[i];
        for (int i = 0; i < 40; i++) start[i] = st_[i],end[i] = ed_[i];
        date[0] = d0;
        date[1] = d1;
        time[0] = t0;
        time[1] = t1;
        duration = dur;
        price = price_;
        seat = seat_;
    }
    friend std::ostream& operator <<(std::ostream &os, Ticket &t) {
        os << t.trainID << " " << t.start << " " << t.date[0] << " " <<
            t.time[0] << " -> " << t.end << " " << t.date[1] << " " <<
                t.time[1] << " " << t.price << " " << t.seat;
        return os;
    }
};

#endif //TICKET_SYSTEM_2026_TICKET_H
//
// Created by HTY on 2026/5/25.
//

#ifndef TICKET_SYSTEM_2026_TRAIN_H
#define TICKET_SYSTEM_2026_TRAIN_H
#include "vector.h"
#include<iostream>
#include <string>
class Train {
public:
    char trainID[20];
    int stationNum;
    int seatNum;
    char stations[100][40];
    int prices[99];
    struct Time {
        int hour;
        int minute;
        Time() {
            hour = minute = 0;
        }
        bool operator <(Time &other) const {
            if (hour < other.hour) return true;
            if (hour > other.hour) return false;
            if (minute < other.minute) return true;
            return false;
        }
        bool operator <=(Time &other) const {
            if (hour < other.hour) return true;
            if (hour > other.hour) return false;
            if (minute <= other.minute) return true;
            return false;
        }
        friend std::ostream& operator <<(std::ostream& os,Time &t) {
            if (t.hour < 10) os << '0' << t.hour;
            else os << t.hour;
            os << ':';
            if (t.minute < 10) os << '0' << t.minute;
            else os << t.minute;
            return os;
        }
    };
    Time startTime;
    int travelTimes[99];
    int stopoverTimes[98];
    struct Date {
        int month;
        int day;
        Date() {
            month = 1,day = 1;
        }
        Date(int m,int d) {
            month = m,day = d;
        }
        bool operator <(Date &other) const {
            if (month < other.month) return true;
            if (month > other.month) return false;
            if (day < other.day) return true;
            return false;
        }
        bool operator <=(Date &other) const {
            if (month < other.month) return true;
            if (month > other.month) return false;
            if (day <= other.day) return true;
            return false;
        }
        Date operator +(int n) {
            day += n;
            if (day > 30) {
                if (month == 6) {
                    day -= 30;
                    month = 7;
                }
                else if (day > 31) {
                    day -= 31;
                    month++;
                }
            }
            return *this;
        }
        friend std::ostream& operator <<(std::ostream& os,Date &d) {
            if (d.month < 10) os << '0' << d.month;
            else os << d.month;
            os << '-';
            if (d.day < 10) os << '0' << d.day;
            else os << d.day;
            return os;
        }
    };
    Date saleDate[2];
    char type;
    bool release = false;
    int remainTickets[92][99];// 发车日期6.1~8.31 + 区间
    Train(){}
    Train(std::string &i,int n,int m,sjtu::vector<std::string> &s,sjtu::vector<int> &p,Time &x,sjtu::vector<int> &t,sjtu::vector<int> &o,sjtu::vector<std::string> &d,std::string &y) {
        size_t num = i.size();
        for (int j = 0; j < num; j++) trainID[j] = i[j];
        for (size_t j = num; j < 20; j++) trainID[j] = '\0';
        stationNum = n;
        seatNum = m;
        for (int j = 0; j < stationNum; j++) {
            num = s[j].size();
            for (int k = 0; k < num; k++) stations[j][k] = s[j][k];
            for (size_t k = num; k < 40; k++) stations[j][k] = '\0';
        }
        for (int j = 0; j < stationNum - 1; j++) {
            prices[j] = p[j];
        }
        startTime = x;
        for (int j = 0; j < stationNum - 1; j++) {
            travelTimes[j] = t[j];
        }
        for (int j = 0; j < stationNum - 2; j++) {
            stopoverTimes[j] = o[j];
        }
        saleDate[0] = GetDate(d[0]);
        saleDate[1] = GetDate(d[1]);
        type = y[0];
        release = false;
        for (int j = 0; j < stationNum - 1; j++) {
            for (int k = 0; k < 92; k++) remainTickets[k][j] = seatNum;
        }
    }
    friend bool operator <(Train &a,Train &b) {
        for (int i = 0; i < 20; i++) {
            if (a.trainID[i] < b.trainID[i]) return true;
            if (a.trainID[i] > b.trainID[i]) return false;
        }
        return false;
    }
    friend bool operator >(Train &a,Train &b) {
        for (int i = 0; i < 20; i++) {
            if (a.trainID[i] > b.trainID[i]) return true;
            if (a.trainID[i] < b.trainID[i]) return false;
        }
        return false;
    }
    friend bool operator <=(Train &a,Train &b) {
        for (int i = 0; i < 20; i++) {
            if (a.trainID[i] < b.trainID[i]) return true;
            if (a.trainID[i] > b.trainID[i]) return false;
        }
        return true;
    }
    friend bool operator >=(Train &a,Train &b) {
        for (int i = 0; i < 20; i++) {
            if (a.trainID[i] > b.trainID[i]) return true;
            if (a.trainID[i] < b.trainID[i]) return false;
        }
        return true;
    }
    friend bool operator ==(Train &a,Train &b) {
        for (int i = 0; i < 20; i++) {
            if (a.trainID[i] != b.trainID[i]) return false;
        }
        return true;
    }
    friend bool operator !=(Train &a,Train &b) {
        for (int i = 0; i < 20; i++) {
            if (a.trainID[i] != b.trainID[i]) return true;
        }
        return false;
    }
private:
    Date GetDate(std::string &s) {
        Date date;
        date.month = (s[0] - '0') * 10 + (s[1] - '0');
        date.day = (s[3] - '0') * 10 + (s[4] - '0');
        return date;
    }
};
#endif//TICKET_SYSTEM_2026_TRAIN_H

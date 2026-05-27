//
// Created by HTY on 2026/5/25.
//

#ifndef TICKET_SYSTEM_2026_TRAIN_H
#define TICKET_SYSTEM_2026_TRAIN_H
struct Train {
    char trainID[20];
    int stationNum;
    char stations[100][40];
    int seatNum;
    int prices[99];
    struct Time {
        int hour;
        int minute;
    };
    Time startTime;
    int travelTimes[99];
    int stopoverTimes[98];
    struct Date {
        int month;
        int day;
    };
    Date saleDate[2];
    bool release;
};
#endif //TICKET_SYSTEM_2026_TRAIN_H
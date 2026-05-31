//
// Created by HTY on 2026/5/25.
//
#include "../include/User.h"
#include "../include/Train.h"
#include "../include/TrainSystem.h"
#include "../include/vector.h"
#include "../include/Account.h"
#include "../include/Tool_functions.h"
#include "../include/Ticket.h"
#include "../include/Order.h"
#include "../include/TicketManagement.h"
#include<iostream>
#include<string>
#include<cstdio>
#include<cmath>
#include<fstream>
#include<filesystem>
sjtu::vector<User> login;
char record[20];
int main() {
    Account ac;
    TrainSystem ts;
    TicketManagement tm;
    ac.Init();
    ts.Init();
    tm.Init();
    while (true) {
        bool valid = true;
        std::string input;
        getline(std::cin,input);
        std::stringstream instruction(input);
        std::string timestamp,cmd;
        instruction >> timestamp;
        instruction >> cmd;
        if (cmd == "exit") {
            ac.Close();
            ts.Close();
            tm.Close();
            std::cout << timestamp << " bye";
            break;
        }
        if (cmd == "clean") {
            // 删除所有数据
            ac.Clear();
            ts.Clear();
            tm.Clear();
            login.clear();
            std::cout << timestamp << " " << 0 << '\n';
        }
        // 用户账户操作模块
        {
            if (cmd == "add_user") {
                std::string c,u,pp,n,m;
                std::string prev,tmp;
                int g;
                while (instruction >> tmp) {
                    if (tmp == "-c" || tmp == "-u" || tmp == "-p" || tmp == "-n" || tmp == "-m" || tmp == "-g") {
                        prev = tmp;
                        continue;
                    }
                    else {
                        if (prev == "-c") c = tmp;
                        if (prev == "-u") u = tmp;
                        if (prev == "-p") pp = tmp;
                        if (prev == "-n") n = tmp;
                        if (prev == "-m") m = tmp;
                        if (prev == "-g") g = StringToInt(tmp);
                    }
                }
                // std::cerr << u << " " << p << " " << n << " " << m << " " << g << '\n';
                User new_user(u,pp,n,m,g);
                if (!ac.HasAnyUser()) {
                    // 创建第一个用户
                    new_user.privilege = 10;
                    ac.AddUser(new_user);
                    std::cout << timestamp << " " << 0 << '\n';
                }
                else {
                    // 检查合法性
                    if (ac.ExistThisUser(new_user.username)) valid = false;
                    else {
                        bool has_login = false;
                        User cur;
                        size_t num = login.size();
                        for (int i = 0; i < num; i++) {
                            if (login[i].username == c) {
                                has_login = true;
                                cur = login[i];
                            }
                        }
                        if (!has_login || cur.privilege <= g) valid = false;
                        else {
                            ac.AddUser(new_user);
                            std::cout << timestamp << " " << 0 << '\n';
                        }
                    }
                }
            }
            if (cmd == "login") {
                std::string u,pp;
                std::string tmp,prev;
                while (instruction >> tmp) {
                    if (tmp == "-u" || tmp == "-p") {
                        prev = tmp;
                        continue;
                    }
                    if (prev == "-u") u = tmp;
                    if (prev == "-p") pp = tmp;
                }
                char tmp_u[20];
                StringToChar20(tmp_u,u);
                if (!ac.ExistThisUser(tmp_u)) valid = false;
                else {
                    User cur = ac.GetUser(tmp_u);
                    if (cur.password != pp) valid = false;
                    else {
                        size_t num = login.size();
                        for (int i = 0; i < num; i++) {
                            if (login[i].username == u) {
                                valid = false;
                                break;
                            }
                        }
                        if (valid) {
                            login.push_back(cur);
                            std::cout << timestamp << " " << 0 << '\n';
                        }
                    }
                }
            }
            if (cmd == "logout") {
                std::string u,tmp;
                instruction >> tmp;
                if (tmp == "-u") instruction >> u;
                bool exist = false;
                for (auto it = login.begin(); it != login.end(); it++) {
                    User tmp_u = *it;
                    if (tmp_u.username == u) {
                        exist = true;
                        login.erase(it);
                        break;
                    }
                }
                if (!exist) {
                    valid = false;
                }
                else {
                    std::cout << timestamp << " " << 0 << '\n';
                }
            }
            if (cmd == "query_profile") {
                std::string c,u,tmp,prev;
                while (instruction >> tmp) {
                    if (tmp == "-c" || tmp == "-u") {
                        prev = tmp;
                        continue;
                    }
                    if (prev == "-c") c = tmp;
                    if (prev == "-u") u = tmp;
                }
                char tmp_username[20];
                StringToChar20(tmp_username,u);
                User cur;
                bool exist = false;
                for (auto tmp_u : login) {
                    if (tmp_u.username == c) {
                        exist = true;
                        cur = tmp_u;
                        break;
                    }
                }
                if (!exist || !ac.ExistThisUser(tmp_username)) valid = false;
                else {
                    User query = ac.GetUser(tmp_username);
                    if (cur.privilege < query.privilege || cur.privilege == query.privilege && c != u) valid = false;
                    else {
                        //std::cerr << cur << '\n';
                        std::cout << timestamp << " " << query << '\n';
                    }
                }
            }
            if (cmd == "modify_profile") {
                std::string c,u,pp,n,m;
                int g = -1;
                std::string tmp,prev;
                while (instruction >> tmp) {
                    if (tmp == "-c" || tmp == "-u" || tmp == "-p" || tmp == "-n" || tmp == "-m" || tmp == "-g") {
                        prev = tmp;
                        continue;
                    }
                    if (prev == "-c") c = tmp;
                    if (prev == "-u") u = tmp;
                    if (prev == "-p") pp = tmp;
                    if (prev == "-n") n = tmp;
                    if (prev == "-m") m = tmp;
                    if (prev == "-g") g = StringToInt(tmp);
                }
                User cur;
                bool exist = false;
                for (auto tmp_u : login) {
                    if (tmp_u.username == c) {
                        cur = tmp_u;
                        exist = true;
                        break;
                    }
                }
                if (!exist) valid = false;
                else {
                    char tmp_username[20];
                    StringToChar20(tmp_username,u);
                    if (!ac.ExistThisUser(tmp_username)) valid = false;
                    else {
                        User old = ac.GetUser(tmp_username);
                        if (pp.empty()) pp = old.password;
                        if (n.empty()) n = old.name;
                        if (m.empty()) m = old.mailAddr;
                        if (g == -1) g = old.privilege;
                        if (cur.privilege < old.privilege || g >= cur.privilege) valid = false;
                        else {
                            ac.DeleteUser(old);
                            User modify(u,pp,n,m,g);
                            ac.AddUser(modify);
                            std::cout << timestamp << " " << modify << '\n';
                        }
                    }
                }
            }
        }
        // ===
        // 火车信息模块
        {
            if (cmd == "add_train") {
                std::string tmp,prev;
                std::string i,y;
                int n,m;
                sjtu::vector<std::string> s,d;
                sjtu::vector<int> pp,t,o;
                Train::Time x;
                while (instruction >> tmp) {
                    if (tmp == "-i" || tmp == "-n" || tmp == "-m" || tmp == "-s" || tmp == "-p" || tmp == "-x" || tmp == "-t" || tmp == "-o" || tmp == "-d" || tmp == "-y") {
                        prev = tmp;
                        continue;
                    }
                    if (prev == "-i") i = tmp;
                    if (prev == "-n") n = StringToInt(tmp);
                    if (prev == "-m") m = StringToInt(tmp);
                    if (prev == "-s") s = SeparateString(tmp);
                    if (prev == "-p") pp = SeparateInt(tmp);
                    if (prev == "-x") x = GetTime(tmp);
                    if (prev == "-t") t = SeparateInt(tmp);
                    if (prev == "-o") {
                        if (tmp != "_") o = SeparateInt(tmp);
                    }
                    if (prev == "-d") d = SeparateString(tmp);
                    if (prev == "-y") y = tmp;
                }
                char tmp_id[20];
                StringToChar20(tmp_id,i);
                if (ts.ExistThisTrain(tmp_id)) valid = false;
                else {
                    Train new_train(i,n,m,s,pp,x,t,o,d,y);
                    ts.AddTrain(new_train);
                    //for (int j = 0; j < 20; j++) record[j] = new_train.trainID[j];
                    std::cout << timestamp << " " << 0 << '\n';
                }
            }
            if (cmd == "delete_train") {
                std::string i;
                instruction >> i;
                if (i == "-i") instruction >> i;
                char tmp_t[20];
                StringToChar20(tmp_t,i);
                if (!ts.ExistThisTrain(tmp_t)) valid = false;
                else {
                    Train cur = ts.GetTrain(tmp_t);
                    if (cur.release == true) valid = false;
                    else {
                        ts.DeleteTrain(cur);
                        std::cout << timestamp << " " << 0 << '\n';
                    }
                }
            }
            if (cmd == "release_train") {
                std::string tmp,i;
                instruction >> tmp;
                if (tmp == "-i") instruction >> i;
                char tmp_t[20];
                StringToChar20(tmp_t,i);
                if (!ts.ExistThisTrain(tmp_t)) valid = false;
                else {
                    Train cur = ts.GetTrain(tmp_t);
                    if (cur.release == true) valid = false;
                    else {
                        ts.DeleteTrain(cur);
                        cur.release = true;
                        ts.AddTrain(cur);
                        for (int j = 0; j < cur.stationNum; j++) {
                            tm.AddTrainToStation(cur.stations[j],cur.trainID);
                        }
                        for (int j = 0; j < cur.stationNum; j++) {
                            for (int k = j + 1; k < cur.stationNum; k++) {
                                tm.AddTicket(TicketKey(cur.stations[j],cur.stations[k]),cur.trainID);
                            }
                        }
                        std::cout << timestamp << " " << 0 << '\n';
                    }
                }
            }
            if (cmd == "query_train") {
                std::string i,tmp,prev;
                Train::Date d;
                while (instruction >> tmp) {
                    if (tmp == "-i" || tmp == "-d") {
                        prev = tmp;
                        continue;
                    }
                    if (prev == "-i") i = tmp;
                    if (prev == "-d") d = GetDate(tmp);
                }
                char tmp_t[20];
                StringToChar20(tmp_t,i);
                if (!ts.ExistThisTrain(tmp_t)) valid = false;
                else {
                    Train cur = ts.GetTrain(tmp_t);
                    //std::cerr << d << " " << cur.saleDate[0] << " " << cur.saleDate[1] << '\n';
                    if (d < cur.saleDate[0] || cur.saleDate[1] < d) valid = false;
                    else {
                        Train::Date tmp_date = d;
                        Train::Time tmp_time = cur.startTime;
                        int day_id = GetIntervalDays(Train::Date(6,1),tmp_date);
                        int total_cost = 0;
                        std::cout << timestamp << " " << cur.trainID << " " << cur.type << '\n';
                        for (int j = 0; j < cur.stationNum; j++) {
                            std::cout << cur.stations[j] << " ";
                            if (j == 0) {
                                // 始发站
                                std::cout << "xx-xx xx:xx -> " << tmp_date << " " << tmp_time << " " << total_cost << " " << cur.remainTickets[day_id][j] << '\n';
                                ComputeDateAndTime(tmp_date,tmp_time,cur.travelTimes[j]);
                                total_cost += cur.prices[j];
                            }
                            else if (j == cur.stationNum - 1) {
                                // 终点站
                                std::cout << tmp_date << " " << tmp_time << " -> xx-xx xx:xx " << total_cost << " x\n";
                            }
                            else {
                                std::cout << tmp_date << " " << tmp_time << " -> ";
                                ComputeDateAndTime(tmp_date,tmp_time,cur.stopoverTimes[j - 1]);
                                std::cout << tmp_date << " " << tmp_time << " " << total_cost << " " << cur.remainTickets[day_id][j] << '\n';
                                ComputeDateAndTime(tmp_date,tmp_time,cur.travelTimes[j]);
                                total_cost += cur.prices[j];
                            }
                        }
                    }
                }
            }
        }
        // ===
        // 火车票务模块
        if (cmd == "query_ticket") {
            std::string s,t,pp = "time",tmp,prev;
            Train::Date d;
            while (instruction >> tmp) {
                if (tmp == "-s" || tmp == "-t" || tmp == "-d" || tmp == "-p") {
                    prev = tmp;
                    continue;
                }
                if (prev == "-s") s = tmp;
                if (prev == "-t") t = tmp;
                if (prev == "-p") pp = tmp;
                if (prev == "-d") d = GetDate(tmp);
            }
            sjtu::vector<Key20> qual_trainID = tm.GetQualifiedTrains(s,t);
            sjtu::vector<Ticket> qual_ticket;
            int cnt = 0;
            for (auto & it : qual_trainID) {
                Train cur = ts.GetTrain(it.cont);
                Train::Date tmp_date0 = cur.saleDate[0],tmp_date1 = cur.saleDate[1];
                Train::Time tmp_time0 = cur.startTime,tmp_time1 = cur.startTime;
                int st = 0,ed = 0;// 遍历找到起终点序号
                while (cur.stations[st] != s) {
                    ComputeDateAndTime(tmp_date0,tmp_time0,cur.travelTimes[st] + cur.stopoverTimes[st]);
                    ComputeDateAndTime(tmp_date1,tmp_time1,cur.travelTimes[st] + cur.stopoverTimes[st]);
                    st++;
                }
                if (tmp_date0 <= d && d <= tmp_date1) {
                    Train::Time time_ar = tmp_time0;
                    Train::Date day_ar = d;
                    int dur = 0;
                    int day_id = GetIntervalDays(tmp_date0,d);
                    dur += cur.travelTimes[st];
                    ComputeDateAndTime(day_ar,time_ar,cur.travelTimes[st]);
                    ed = st + 1;
                    int min_seat = cur.remainTickets[day_id][st];
                    int total_price = cur.prices[st];
                    while (cur.stations[ed] != t) {
                        min_seat = std::min(min_seat,cur.remainTickets[day_id][ed]);
                        total_price += cur.prices[ed];
                        dur += cur.stopoverTimes[ed - 1] + cur.travelTimes[ed];
                        ComputeDateAndTime(day_ar,time_ar,cur.stopoverTimes[ed - 1] + cur.travelTimes[ed]);
                        ed++;
                    }
                    Ticket ticket(cur.trainID,cur.stations[st],cur.stations[ed],d,tmp_time0,day_ar,time_ar,dur,total_price,min_seat);
                    qual_ticket.push_back(ticket);
                    cnt++;
                }
            }
            if (pp == "time") {
                tm.SortByTime(qual_ticket);
            }
            if (pp == "cost") {
                tm.SortByPrice(qual_ticket);
            }
            std::cout << timestamp << " " << cnt << '\n';
            for (int i = 0; i < cnt; i++) {
                std::cout << qual_ticket[i] << '\n';
            }
        }
        if (cmd == "query_transfer") {
            std::string s,t,pp = "time",tmp,prev;
            Train::Date d;
            while (instruction >> tmp) {
                if (tmp == "-s" || tmp == "-t" || tmp == "-d" || tmp == "-p") {
                    prev = tmp;
                    continue;
                }
                if (prev == "-s") s = tmp;
                if (prev == "-t") t = tmp;
                if (prev == "-p") pp = tmp;
                if (prev == "-d") d = GetDate(tmp);
            }
            char tmp_station[40];
            bool can_transfer = false;
            Ticket best_ticket1,best_ticket2;
            int best_price = 1e9;
            int best_dur = 1e9;
            StringToChar40(tmp_station,s);
            sjtu::vector<Key20> qual_trainID = tm.GetTrainsFromHere(tmp_station);
            for (auto it : qual_trainID) {
                Train train1 = ts.GetTrain(it.cont);
                int st1 = 0;
                Train::Date tmp_date0 = train1.saleDate[0],tmp_date1 = train1.saleDate[1];
                Train::Time tmp_time0 = train1.startTime,tmp_time1 = train1.startTime;
                while (train1.stations[st1] != s) {
                    ComputeDateAndTime(tmp_date0,tmp_time0,train1.travelTimes[st1] + train1.stopoverTimes[st1]);
                    ComputeDateAndTime(tmp_date1,tmp_time1,train1.travelTimes[st1] + train1.stopoverTimes[st1]);
                    st1++;
                }
                if (tmp_date0 <= d && d <= tmp_date1) {
                    Train::Date date = d;
                    Train::Time time = tmp_time0;
                    int day_id1 = GetIntervalDays(tmp_date0,d);
                    int dur1 = train1.travelTimes[st1];
                    int price1 = train1.prices[st1];
                    int min_seat1 = train1.remainTickets[day_id1][st1];
                    ComputeDateAndTime(date,time,train1.travelTimes[st1]);
                    for (int mid = st1 + 1; mid < train1.stationNum; mid++) {
                        // 查询mid到t的车票
                        Ticket ticket1(it.cont,train1.stations[st1],train1.stations[mid],d,tmp_time0,date,time,dur1,price1,min_seat1);
                        sjtu::vector<Key20> ticket2_trainID = tm.GetQualifiedTrains(std::string(train1.stations[mid]),t);
                        // 把ticket2_trainID遍历一遍，找到符合要求的ticket2，计算1+2的组合 存到一个变量里,每次比较取最优
                        for (auto it2 : ticket2_trainID) {
                            // 不能同一辆车
                            if (it2.cont == std::string(ticket1.trainID)) continue;
                            Train train2 = ts.GetTrain(it2.cont);
                            int st2 = 0;
                            Train::Date tmp_date2 = train2.saleDate[0], tmp_date3 = train2.saleDate[1];
                            Train::Time tmp_time2 = train2.startTime, tmp_time3 = train2.startTime;
                            while (train2.stations[st2] != std::string(train1.stations[mid])) {
                                ComputeDateAndTime(tmp_date2,tmp_time2,train2.travelTimes[st2] + train2.stopoverTimes[st2]);
                                ComputeDateAndTime(tmp_date3,tmp_time3,train2.travelTimes[st2] + train2.stopoverTimes[st2]);
                                st2++;
                            }
                            if (TimeCompare(tmp_date3,tmp_time3,ticket1.date[1],ticket1.time[1])) continue;
                            else {
                                can_transfer = true;
                                int gap_dur;
                                Train::Date date2,date2_0;
                                Train::Time time2,time2_0;
                                if (TimeCompare(ticket1.date[1],ticket1.time[1],tmp_date2,tmp_time2)) {
                                    date2_0 = date2 = tmp_date2;
                                    time2_0 = time2 = tmp_time2;
                                }
                                else if (ticket1.time[1] <= tmp_time2) {
                                    date2_0 = date2 = ticket1.date[1];
                                    time2_0 = time2 = tmp_time2;
                                }
                                else {
                                    date2_0 = date2 = ticket1.date[1] + 1;
                                    time2_0 = time2 = tmp_time2;
                                }
                                gap_dur = GetInterval(ticket1.date[1],ticket1.time[1],date2,time2);
                                int dur2 = train2.travelTimes[st2];
                                int price2 = train2.prices[st2];
                                int day_id2 = GetIntervalDays(tmp_date2,date2);
                                int min_seat2 = train2.remainTickets[day_id2][st2];
                                ComputeDateAndTime(date2,time2,train2.travelTimes[st2]);
                                int ed2 = st2 + 1;
                                while (train2.stations[ed2] != t) {
                                    dur2 += train2.travelTimes[ed2] + train2.stopoverTimes[ed2];
                                    price2 += train2.prices[ed2];
                                    min_seat2 = std::min(min_seat2,train2.remainTickets[day_id2][ed2]);
                                    ComputeDateAndTime(date2,time2,train2.travelTimes[ed2] + train2.stopoverTimes[ed2]);
                                    ed2++;
                                }
                                Ticket ticket2(train2.trainID,train1.stations[mid],train2.stations[ed2],date2_0,time2_0,date2,time2,dur2,price2,min_seat2);
                                int dur = dur1 + gap_dur + dur2;
                                int total_price = price1 + price2;
                                if (pp == "time") {
                                    if (dur < best_dur || dur == best_dur && total_price < best_price
                                        || dur == best_dur && total_price == best_price && std::string(ticket1.trainID) < best_ticket1.trainID
                                        || dur == best_dur && total_price == best_price && std::string(ticket1.trainID) == best_ticket1.trainID && std::string(ticket2.trainID) < best_ticket2.trainID) {
                                        best_ticket1 = ticket1;
                                        best_ticket2 = ticket2;
                                    }
                                }
                                if (pp == "price") {
                                    if (total_price < best_price || total_price == best_price && dur < best_dur
                                        || dur == best_dur && total_price == best_price && std::string(ticket1.trainID) < best_ticket1.trainID
                                        || dur == best_dur && total_price == best_price && std::string(ticket1.trainID) == best_ticket1.trainID && std::string(ticket2.trainID) < best_ticket2.trainID) {
                                        best_ticket1 = ticket1;
                                        best_ticket2 = ticket2;
                                        }
                                }
                            }
                        }
                        if (mid == train1.stationNum - 1) break;
                        dur1 += train1.travelTimes[mid] + train1.stopoverTimes[mid];
                        price1 += train1.prices[mid];
                        min_seat1 = std::min(min_seat1,train1.remainTickets[day_id1][mid]);
                        ComputeDateAndTime(date,time,train1.travelTimes[mid] + train1.stopoverTimes[mid]);
                    }
                }
            }
            if (!can_transfer) std::cout << timestamp << " " << 0 << '\n';
            else {
                // 输出换乘信息
                std::cout << timestamp << " " << best_ticket1 << '\n';
                std::cout << best_ticket2 << '\n';
            }
        }
        if (cmd == "buy_ticket") {
            std::string u,i,f,t,q = "false",tmp,prev;
            int n;
            Train::Date d;
            while (instruction >> tmp) {
                if (tmp == "-u" || tmp == "-i" || tmp == "-d" || tmp == "-n" || tmp == "-f" || tmp == "-f" || tmp == "-t" || tmp == "-q") {
                    prev = tmp;
                    continue;
                }
                if (prev == "-u") u = tmp;
                if (prev == "-i") i = tmp;
                if (prev == "-d") d = GetDate(tmp);
                if (prev == "-f") f = tmp;
                if (prev == "-t") t = tmp;
                if (prev == "-n") n = StringToInt(tmp);
                if (prev == "-q") q = tmp;
            }
            char tmp_train_id[20],tmp_user[20];
            StringToChar20(tmp_train_id,i);
            User cur_user;
            bool exist = false;
            for (auto & it : login) {
                if (it.username == u) {
                    exist = true;
                    cur_user = it;
                    break;
                }
            }
            if (!exist) valid = false;
            else {
                if (!ts.ExistThisTrain(tmp_train_id)) valid = false;
                else {
                    Train cur_train = ts.GetTrain(tmp_train_id);
                    if (!cur_train.release) valid = false;
                    else {
                        Train::Date tmp_date0 = cur_train.saleDate[0], tmp_date1 = cur_train.saleDate[1];
                        Train::Time tmp_time0 = cur_train.startTime, tmp_time1 = cur_train.startTime;
                        int st = 0;
                        while (cur_train.stations[st] != f && st < cur_train.stationNum) {
                            ComputeDateAndTime(tmp_date0,tmp_time0,cur_train.travelTimes[st] + cur_train.stopoverTimes[st]);
                            ComputeDateAndTime(tmp_date1,tmp_time1,cur_train.travelTimes[st] + cur_train.stopoverTimes[st]);
                            st++;
                        }
                        if (tmp_date0 <= d && d <= tmp_date1) {
                            Train::Date date = d;
                            Train::Time time = tmp_time0;
                            int dur = 0;
                            int day_id = GetIntervalDays(tmp_date0,d);
                            int price = cur_train.prices[st];
                            int min_seat = cur_train.remainTickets[day_id][st];
                            int ed = st + 1;
                            dur += cur_train.travelTimes[st];
                            ComputeDateAndTime(date,time,cur_train.travelTimes[st]);
                            while (cur_train.stations[ed] != t && ed < cur_train.stationNum) {
                                min_seat = std::min(min_seat,cur_train.remainTickets[day_id][ed]);
                                price += cur_train.prices[ed];
                                dur += cur_train.stopoverTimes[ed] + cur_train.travelTimes[ed];
                                ComputeDateAndTime(date,time,cur_train.stopoverTimes[ed - 1] + cur_train.travelTimes[ed]);
                                ed++;
                            }
                            if (ed >= cur_train.stationNum) valid = false;
                            else {
                                Ticket ticket(cur_train.trainID,cur_train.stations[st],cur_train.stations[ed],d,tmp_time0,date,time,dur,price,min_seat);
                                if (n <= min_seat) {
                                    ts.DeleteTrain(cur_train);
                                    std::cout << timestamp << " " << price * n << '\n';
                                    for (int j = st; j < ed; j++) cur_train.remainTickets[day_id][j] -= n;
                                    ts.AddTrain(cur_train);
                                    Order order(cur_user.username,GetTimeStamp(timestamp),Order::Status::success,ticket,n);
                                    ac.AddUserOrder(order);
                                }
                                else if (q == "false") valid = false;
                                else {
                                    std::cout << timestamp << " " << "queue" << '\n';
                                    Order order(cur_user.username,GetTimeStamp(timestamp),Order::Status::pending,ticket,n);
                                    ac.AddUserOrder(order);
                                    ts.AddTrainOrder(order);
                                }
                            }
                        }
                        else valid = false;
                    }
                }
            }
        }
        if (cmd == "query_order") {
            std::string tmp,u;
            instruction >> tmp;
            if (tmp == "-u") instruction >> u;
            bool exist = false;
            User cur;
            for (auto it : login) {
                if (it.username == u) {
                    exist = true;
                    cur = it;
                    break;
                }
            }
            if (!exist) valid = false;
            else {
                sjtu::vector<Order> orders = ac.GetAllUserOrders(cur.username);
                int size_ = orders.size();
                std::cout << timestamp << " " << size_ << '\n';
                for (int i = size_ - 1; i >= 0; i--) {
                    std::cout << orders[i] << '\n';
                }
            }
        }
        if (cmd == "refund_ticket") {
            std::string u,tmp,prev;
            int n = 1;
            while (instruction >> tmp) {
                if (tmp == "-u" || tmp == "-n") {
                    prev = tmp;
                    continue;
                }
                if (prev == "-u") u = tmp;
                if (prev == "-n") n = StringToInt(tmp);
            }
            bool exist = false;
            User cur;
            for (auto it : login) {
                if (it.username == u) {
                    exist = true;
                    cur = it;
                    break;
                }
            }
            if (!exist) valid = false;
            else {
                sjtu::vector<Order> orders = ac.GetAllUserOrders(cur.username);
                size_t size_ = orders.size();
                if (n > 0 && n <= size_) {
                    Order refund = orders[size_ - n];
                    if (refund.status == Order::Status::refunded) valid = false;
                    else {
                        std::cout << timestamp << " " << 0 << '\n';
                        bool pending = false;
                        if (refund.status == Order::Status::pending) {
                            pending = true;
                            ts.DeleteTrainOrder(refund);
                        }
                        ac.DeleteUserOrder(refund);
                        refund.status = Order::Status::refunded;
                        ac.AddUserOrder(refund);
                    // 把退掉的位置加回到列车信息里面
                        if (!pending) {
                            // 买到了再退
                            Train train = ts.GetTrain(refund.ticket.trainID);
                            int st = 0;
                            Train::Date tmp_date = train.saleDate[0];
                            Train::Time tmp_time = train.startTime;
                            while (std::string(train.stations[st]) != refund.ticket.start) {
                                ComputeDateAndTime(tmp_date,tmp_time,train.travelTimes[st] + train.stopoverTimes[st]);
                                st++;
                            }
                            int day_id = GetIntervalDays(tmp_date,refund.ticket.date[0]);
                            int ed = st;
                            while (std::string(train.stations[ed]) != refund.ticket.end) {
                                train.remainTickets[day_id][ed] += refund.number;
                                ed++;
                            }
                            // 处理补票
                            sjtu::vector<Order> order_of_this_train = ts.GetAllTrainOrders(refund.ticket.trainID);
                            for (auto it : order_of_this_train) {
                                // 检查每个还存在的订单是否可以成立 按照时间从先到后的顺序
                                int st2 = 0;
                                tmp_date = train.saleDate[0];
                                tmp_time = train.startTime;
                                while (std::string(train.stations[st2]) != it.ticket.start) {
                                    ComputeDateAndTime(tmp_date,tmp_time,train.travelTimes[st2] + train.stopoverTimes[st2]);
                                    st2++;
                                }
                                int day_id2 = GetIntervalDays(tmp_date,it.ticket.date[0]);
                                int min_seat = train.remainTickets[day_id2][st2];
                                int ed2 = st2 + 1;
                                while (std::string(train.stations[ed2]) != it.ticket.end) {
                                    min_seat = std::min(min_seat,train.remainTickets[day_id2][ed2]);
                                    ed2++;
                                }
                                if (min_seat <= it.number) {
                                    // 成功候补
                                    it.status = Order::Status::success;
                                    for (int i = st2; i < ed2; i++) train.remainTickets[day_id2][i] -= it.number;
                                    ac.DeleteUserOrder(it);
                                    ac.AddUserOrder(it);
                                    ts.DeleteTrainOrder(it);
                                }
                            }
                            ts.DeleteTrain(train);
                            ts.AddTrain(train);
                        }
                    }
                }
                else valid = false;
            }
        }
        // ===
        if (!valid) {
            std::cout << timestamp << " "  << -1 << "\n";
        }
    }
    return 0;
}
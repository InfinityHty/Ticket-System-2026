//
// Created by HTY on 2026/5/26.
//
// 一些处理不同类型转换的工具函数
#ifndef TICKET_SYSTEM_2026_CONVERT_FUNCTIONS_H
#define TICKET_SYSTEM_2026_CONVERT_FUNCTIONS_H
#include<string>
#include "vector.h"
#include "Train.h"
const long long M = 2305843009213693951;
const int p = 101;

inline long long HashString(std::string &s) {
    long long hash[40];
    size_t num = s.size();
    hash[0] = 0;
    for (int i = 1; i < num; i++) hash[i] = (hash[i - 1] * p + s[i]) % M;
    return hash[num - 1];
}
inline long long HashChar(const char *s) {
    long long hash[40];
    hash[0] = 0;
    int i = 1;
    while (s[i] != '\n') {
        hash[i] = (hash[i - 1] * p + s[i]) % M;
        i++;
    }
    return hash[i - 1];
}
inline int StringToInt(const std::string &s) {
    int ans = 0;
    for (char c : s) {
        ans = ans * 10 + c - '0';
    }
    return ans;
}

inline void StringToChar20(char *ans, const std::string &s) {
    for (int i = 0; i < 20; i++) ans[i] = '\0';
    int i = 0;
    for (char c : s) ans[i++] = c;
}

inline void StringToChar40(char *ans, const std::string &s) {
    for (int i = 0; i < 40; i++) ans[i] = '\0';
    int i = 0;
    for (char c : s) ans[i++] = c;
}

inline sjtu::vector<std::string> SeparateString(const std::string &s) {
    std::string tmp{};
    sjtu::vector<std::string> ans{};
    for (char c : s) {
        if (c == '|') {
            ans.push_back(tmp);
            tmp.clear();
        }
        else tmp.push_back(c);
    }
    ans.push_back(tmp);
    return ans;
}

inline sjtu::vector<int> SeparateInt(const std::string &s) {
    int tmp = 0;
    sjtu::vector<int> ans{};
    for (char c : s) {
        if (c == '|') {
            ans.push_back(tmp);
            tmp = 0;
        }
        else tmp = tmp * 10 + c - '0';
    }
    ans.push_back(tmp);
    return ans;
}

inline Train::Time GetTime(const std::string &s) {
    Train::Time time;
    time.hour = (s[0] - '0') * 10 + (s[1] - '0');
    time.minute = (s[3] - '0') * 10 + (s[4] - '0');
    return time;
}

inline Train::Date GetDate(std::string &s) {
    Train::Date date;
    date.month = (s[0] - '0') * 10 + (s[1] - '0');
    date.day = (s[3] - '0') * 10 + (s[4] - '0');
    return date;
}

inline bool TimeCompare(Train::Date &d1,Train::Time &t1,Train::Date &d2,Train::Time &t2) {
    // <
    if (d1 < d2) return true;
    if (d2 < d1) return false;
    if (t1 < t2) return true;
    if (t2 < t1) return false;
    return false;
}

inline void ComputeDateAndTime(Train::Date &d1,Train::Time &t1,int interval) {
    // d1-t1 经过 interval 分钟后
    t1.minute += interval;
    t1.hour += t1.minute / 60;
    t1.minute %= 60;
    d1.day += t1.hour / 24;
    t1.hour %= 24;
    // 2026年
    if (d1.month == 1 || d1.month == 3 || d1.month == 5 || d1.month == 7 || d1.month == 8 || d1.month == 10 || d1.month == 12) {
        if (d1.day > 31) {
            d1.day -= 31;
            d1.month++;
        }
    }
    else if (d1.month == 2) {
        if (d1.day > 28) {
            d1.day -= 28;
            d1.month++;
        }
    }
    else {
        if (d1.day > 30) {
            d1.day -= 30;
            d1.month++;
        }
    }
}
// 默认b在a之后
int GetIntervalDays(const Train::Date &a,const Train::Date &b) {
    if (a.month == b.month) {
        return b.day - a.day;
    }
    if (b.month == 7) {
        return b.day + 30 - a.day;
    }
    if (b.month == 8 && a.month == 7) return b.day + 31 - a.day;
    if (b.month == 8 && a.month == 6) return b.day + 30 - a.day + 31;
}

inline int GetInterval(Train::Date &d1,Train::Time &t1,Train::Date &d2,Train::Time &t2) {
    // 计算间隔的时间（min）
    // 一趟车运行时间不超过72 * 60min
    int ans = 0;
    if (d1.month == d2.month) ans += 24 * 60 * (d2.day - d1.day);
    else if (d1.month == 6 && d2.month == 7) {
        ans += (d2.day + 30 - d1.day) * 24 * 60;
    }
    else if (d1.month == 7 && d2.month == 8) {
        ans += (d2.day + 31 - d1.day) * 24 * 60;
    }
    ans += 60 * (t2.hour - t1.hour);
    ans += t2.minute - t1.minute;
    return ans;
}

inline int GetTimeStamp(std::string &s) {
    const size_t size_ = s.size();
    int ans = 0;
    for (int i = 1; i < size_ - 1; i++) ans = ans * 10 + s[i] - '0';
    return ans;
}
#endif //TICKET_SYSTEM_2026_CONVERT_FUNCTIONS_H
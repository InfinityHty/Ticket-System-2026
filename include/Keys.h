//
// Created by HTY on 2026/5/28.
//

#ifndef TICKET_SYSTEM_2026_KEYS_H
#define TICKET_SYSTEM_2026_KEYS_H
#include<string>
#include<iostream>
struct Key20 {
    char cont[21];
    Key20() {
        for (int i = 0; i <= 20; i++) cont[i] = '\0';
    }
    Key20(const char *other) {
        for (int i = 0; i <= 20; i++) cont[i] = other[i];
    }
    friend bool operator <(Key20 &a,Key20 &b) {
        for (int i = 0; i <= 20; i++) {
            if (a.cont[i] < b.cont[i]) return true;
            if (a.cont[i] > b.cont[i]) return false;
        }
        return false;
    }
    friend bool operator >(Key20 &a,Key20 &b) {
        for (int i = 0; i <= 20; i++) {
            if (a.cont[i] > b.cont[i]) return true;
            if (a.cont[i] < b.cont[i]) return false;
        }
        return false;
    }
    friend bool operator ==(Key20 &a,Key20 &b) {
        for (int i = 0; i <= 20; i++) {
            if (a.cont[i] != b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator <=(Key20 &a,Key20 &b) {
        for (int i = 0; i <= 20; i++) {
            if (a.cont[i] < b.cont[i]) return true;
            if (a.cont[i] > b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator >=(Key20 &a,Key20 &b) {
        for (int i = 0; i <= 20; i++) {
            if (a.cont[i] > b.cont[i]) return true;
            if (a.cont[i] < b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator !=(Key20 &a,Key20 &b) {
        for (int i = 0; i <= 20; i++) {
            if (a.cont[i] != b.cont[i]) return true;
        }
        return false;
    }
    friend std::ostream& operator <<(std::ostream &os,Key20 &k){
        os << k.cont;
        return os;
    }
};
// Key是定长为20位的字符串
struct TicketKey {
    char start[41];
    char end[41];
    TicketKey() {
        for (int i = 0; i <= 40; i++) {
            start[i] = end[i] = '\0';
        }
    }
    TicketKey(const std::string &st, const std::string &ed) {
        for (int i = 0; i < st.size(); i++) start[i] = st[i];
        for (int i = st.size(); i <= 40; i++) start[i] = '\0';
        for (int i = 0; i < ed.size(); i++) end[i] = ed[i];
        for (int i = ed.size(); i <= 40; i++) end[i] = '\0';
    }
    friend bool operator < (TicketKey &a, TicketKey &b) {
        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.start[i]);
            const auto y = static_cast<unsigned char>(b.start[i]);
            if (x < y) return true;
            if (x > y) return false;
        }

        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.end[i]);
            const auto y = static_cast<unsigned char>(b.end[i]);
            if (x < y) return true;
            if (x > y) return false;
        }
        return false;
    }
    friend bool operator > (TicketKey &a, TicketKey &b) {
        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.start[i]);
            const auto y = static_cast<unsigned char>(b.start[i]);
            if (x > y) return true;
            if (x < y) return false;
        }

        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.end[i]);
            const auto y = static_cast<unsigned char>(b.end[i]);
            if (x > y) return true;
            if (x < y) return false;
        }
        return false;
    }
    friend bool operator <= (TicketKey &a, TicketKey &b) {
        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.start[i]);
            const auto y = static_cast<unsigned char>(b.start[i]);
            if (x < y) return true;
            if (x > y) return false;
        }

        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.end[i]);
            const auto y = static_cast<unsigned char>(b.end[i]);
            if (x < y) return true;
            if (x > y) return false;
        }
        return true;
    }
    friend bool operator >= (TicketKey &a, TicketKey &b) {
        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.start[i]);
            const auto y = static_cast<unsigned char>(b.start[i]);
            if (x > y) return true;
            if (x < y) return false;
        }

        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.end[i]);
            const auto y = static_cast<unsigned char>(b.end[i]);
            if (x > y) return true;
            if (x < y) return false;
        }
        return true;
    }
    friend bool operator == (TicketKey &a, TicketKey &b) {
        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.start[i]);
            const auto y = static_cast<unsigned char>(b.start[i]);
            if (x != y) return false;
        }

        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.end[i]);
            const auto y = static_cast<unsigned char>(b.end[i]);
            if (x != y) return false;
        }
        return true;
    }
    friend bool operator != (TicketKey &a, TicketKey &b) {
        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.start[i]);
            const auto y = static_cast<unsigned char>(b.start[i]);
            if (x != y) return true;
        }

        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.end[i]);
            const auto y = static_cast<unsigned char>(b.end[i]);
            if (x != y) return true;
        }
        return false;
    }
};
struct Key40 {
    char cont[41];
    Key40() {
        for (int i = 0; i <= 40; i++) cont[i] = '\0';
    }
    Key40(const char *other) {
        for (int i = 0; i <= 40; i++) cont[i] = other[i];
    }
    friend bool operator <(Key40 &a,Key40 &b) {
        for (int i = 0; i <= 40; i++) {
            const auto x = static_cast<unsigned char>(a.cont[i]);
            const auto y = static_cast<unsigned char>(b.cont[i]);

            if (x < y) return true;
            if (x > y) return false;
        }
        return false;
    }
    friend bool operator >(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            const auto x = static_cast<unsigned char>(a.cont[i]);
            const auto y = static_cast<unsigned char>(b.cont[i]);

            if (x < y) return false;
            if (x > y) return true;
        }
        return false;
    }
    friend bool operator ==(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            const auto x = static_cast<unsigned char>(a.cont[i]);
            const auto y = static_cast<unsigned char>(b.cont[i]);

            if (x != y) return false;
        }
        return true;
    }
    friend bool operator <=(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            const auto x = static_cast<unsigned char>(a.cont[i]);
            const auto y = static_cast<unsigned char>(b.cont[i]);

            if (x < y) return true;
            if (x > y) return false;
        }
        return true;
    }
    friend bool operator >=(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            const auto x = static_cast<unsigned char>(a.cont[i]);
            const auto y = static_cast<unsigned char>(b.cont[i]);

            if (x < y) return false;
            if (x > y) return true;
        }
        return true;
    }
    friend bool operator !=(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            const auto x = static_cast<unsigned char>(a.cont[i]);
            const auto y = static_cast<unsigned char>(b.cont[i]);

            if (x != y) return true;
        }
        return false;
    }
    friend std::ostream& operator <<(std::ostream &os,Key40 &k){
        os << k.cont;
        return os;
    }
};
// 长度为40的字符串
#endif //TICKET_SYSTEM_2026_KEYS_H
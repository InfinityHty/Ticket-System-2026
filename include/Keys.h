//
// Created by HTY on 2026/5/28.
//

#ifndef TICKET_SYSTEM_2026_KEYS_H
#define TICKET_SYSTEM_2026_KEYS_H
struct Key20 {
    char cont[20];
    Key20() {
        for (int i = 0; i < 20; i++) cont[i] = '\0';
    }
    Key20(const char *other) {
        for (int i = 0; i < 20; i++) cont[i] = other[i];
    }
    friend bool operator <(Key20 &a,Key20 &b) {
        for (int i = 0; i < 20; i++) {
            if (a.cont[i] < b.cont[i]) return true;
            if (a.cont[i] > b.cont[i]) return false;
        }
        return false;
    }
    friend bool operator >(Key20 &a,Key20 &b) {
        for (int i = 0; i < 20; i++) {
            if (a.cont[i] > b.cont[i]) return true;
            if (a.cont[i] < b.cont[i]) return false;
        }
        return false;
    }
    friend bool operator ==(Key20 &a,Key20 &b) {
        for (int i = 0; i < 20; i++) {
            if (a.cont[i] != b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator <=(Key20 &a,Key20 &b) {
        for (int i = 0; i < 20; i++) {
            if (a.cont[i] < b.cont[i]) return true;
            if (a.cont[i] > b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator >=(Key20 &a,Key20 &b) {
        for (int i = 0; i < 20; i++) {
            if (a.cont[i] > b.cont[i]) return true;
            if (a.cont[i] < b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator !=(Key20 &a,Key20 &b) {
        for (int i = 0; i < 20; i++) {
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
    char start[40];
    char end[40];
    TicketKey() {
        for (int i = 0; i < 40; i++) {
            start[i] = end[i] = '\0';
        }
    }
    TicketKey(const std::string &st, const std::string &ed) {
        size_t num = st.size();
        for (int i = 0; i < num; i++) start[i] = st[i];
        for (int i = num; i < 40; i++) start[i] = '\0';
        num = ed.size();
        for (int i = 0; i < num; i++) end[i] = ed[i];
        for (int i = num; i < 40; i++) end[i] = '\0';
    }
    friend bool operator < (TicketKey &a, TicketKey &b) {
        for (int i = 0; i < 40; i++) {
            if (a.start[i] < b.start[i]) return true;
            if (a.start[i] > b.start[i]) return false;
        }
        for (int i = 0; i < 40; i++) {
            if (a.end[i] < b.end[i]) return true;
            if (a.end[i] > b.end[i]) return false;
        }
        return false;
    }
    friend bool operator > (TicketKey &a, TicketKey &b) {
        for (int i = 0; i < 40; i++) {
            if (a.start[i] > b.start[i]) return true;
            if (a.start[i] < b.start[i]) return false;
        }
        for (int i = 0; i < 40; i++) {
            if (a.end[i] > b.end[i]) return true;
            if (a.end[i] < b.end[i]) return false;
        }
        return false;
    }
    friend bool operator <= (TicketKey &a, TicketKey &b) {
        for (int i = 0; i < 40; i++) {
            if (a.start[i] < b.start[i]) return true;
            if (a.start[i] > b.start[i]) return false;
        }
        for (int i = 0; i < 40; i++) {
            if (a.end[i] < b.end[i]) return true;
            if (a.end[i] > b.end[i]) return false;
        }
        return true;
    }
    friend bool operator >= (TicketKey &a, TicketKey &b) {
        for (int i = 0; i < 40; i++) {
            if (a.start[i] > b.start[i]) return true;
            if (a.start[i] < b.start[i]) return false;
        }
        for (int i = 0; i < 40; i++) {
            if (a.end[i] > b.end[i]) return true;
            if (a.end[i] < b.end[i]) return false;
        }
        return true;
    }
    friend bool operator == (TicketKey &a, TicketKey &b) {
        for (int i = 0; i < 40; i++) {
            if (a.start[i] != b.start[i]) return false;
        }
        for (int i = 0; i < 40; i++) {
            if (a.end[i] != b.end[i]) return false;
        }
        return true;
    }
    friend bool operator != (TicketKey &a, TicketKey &b) {
        for (int i = 0; i < 40; i++) {
            if (a.start[i] != b.start[i]) return true;
        }
        for (int i = 0; i < 40; i++) {
            if (a.end[i] != b.end[i]) return true;
        }
        return false;
    }
};
struct Key40 {
    char cont[40];
    Key40() {
        for (int i = 0; i < 40; i++) cont[i] = '\0';
    }
    Key40(const char *other) {
        for (int i = 0; i < 40; i++) cont[i] = other[i];
    }
    friend bool operator <(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            if (a.cont[i] < b.cont[i]) return true;
            if (a.cont[i] > b.cont[i]) return false;
        }
        return false;
    }
    friend bool operator >(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            if (a.cont[i] > b.cont[i]) return true;
            if (a.cont[i] < b.cont[i]) return false;
        }
        return false;
    }
    friend bool operator ==(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            if (a.cont[i] != b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator <=(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            if (a.cont[i] < b.cont[i]) return true;
            if (a.cont[i] > b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator >=(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            if (a.cont[i] > b.cont[i]) return true;
            if (a.cont[i] < b.cont[i]) return false;
        }
        return true;
    }
    friend bool operator !=(Key40 &a,Key40 &b) {
        for (int i = 0; i < 40; i++) {
            if (a.cont[i] != b.cont[i]) return true;
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
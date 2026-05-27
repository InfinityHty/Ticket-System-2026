//
// Created by HTY on 2026/5/26.
//

#ifndef TICKET_SYSTEM_2026_CONVERT_FUNCTIONS_H
#define TICKET_SYSTEM_2026_CONVERT_FUNCTIONS_H
#include<string>

inline int StringToInt(std::string &s) {
    int ans = 0;
    for (char c : s) {
        ans = ans * 10 + c - '0';
    }
    return ans;
}

inline void StringToChar20(char *ans,std::string &s) {
    for (int i = 0; i < 20; i++) ans[i] = '\0';
    int i = 0;
    for (char c : s) ans[i++] = c;
}
#endif //TICKET_SYSTEM_2026_CONVERT_FUNCTIONS_H
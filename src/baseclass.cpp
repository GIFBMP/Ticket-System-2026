#include "../include/lexer.h"
#include "../include/baseclass.h"
#include <iostream>

namespace sjtu {
    int mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int proceedTime(const string &str) {
        int hour = stringtoint(str.substr(0, 2));
        int minute = stringtoint(str.substr(3, 2));
        return hour * kMinPerHour + minute;
    }
    int proceedDate(const string &str) {
        int month = stringtoint(str.substr(0, 2));
        int day = stringtoint(str.substr(3 , 2));
        int ret = 0;
        for (int i = 0; i < month - 1; i++) ret += mon[i];
        return (ret + day - 1) * kMinPerDay;
    }
    int getExactMin(int month, int day, int hour, int minute) {
        int ret = 0;
        for (int i = 0; i < month - 1; i++) ret += mon[i];
        return (ret + day) * kMinPerDay + hour * kMinPerHour + minute;
    }
    void printTime(int total_minute) {
        int day = total_minute / kMinPerDay, hour = 0, month = 1;
        total_minute %= kMinPerDay;
        hour = total_minute / kMinPerHour;
        total_minute %= kMinPerHour;
        day++;
        for (int i = 0; i < 12; i++) {
            if (day > mon[i]) day -= mon[i], month++;
            else break;
        }
        if (month < 10) std::cout << '0';
        std::cout << month << '-';
        if (day < 10) std::cout << '0';
        std::cout << day << ' ';
        if (hour < 10) std::cout << '0'; 
        std::cout << hour << ':';
        if (total_minute < 10) std::cout << '0';
        std::cout << total_minute;
    }
    int min (int a, int b) {
        return a < b ? a : b;
    }
    int max (int a, int b) {
        return a > b ? a : b;
    }
    int getDate(int minute) {
        return minute - minute % kMinPerDay;
    }
}
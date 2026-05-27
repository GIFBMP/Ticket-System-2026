#pragma once
#ifndef BASECLASS_H
#define BASECLASS_H
#include <string>
namespace sjtu {
    using std::string;
    template<int len>
    struct Index {
        char s[len + 1];
        Index(const Index &t) {
            for (int i = 0; i <= len; i++)
                s[i] = t.s[i];
        }
        Index(const string &str = "") {
            int sz = str.size();
            for (int i = 0; i < sz && i <= len; i++) s[i] = str[i];
            for (int i = sz; i <= len; i++) s[i] = 0;
        }
        bool empty() const {
            return s[0] == '\0';
        }
        bool operator < (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] < t.s[i]) return true;
                else if (s[i] > t.s[i]) return false;
            return false;
        }
        bool operator <= (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] < t.s[i]) return true;
                else if (s[i] > t.s[i]) return false;
            return true;
        }
        bool operator == (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] != t.s[i]) return false;
            return true;
        }
        bool operator != (const Index &t) const {
            for (int i = 0; i < len; i++)
                if (s[i] != t.s[i]) return true;
            return false;
        }
        string tostring() const {
            string ret = "";
            for (int i = 0; i < len; i++)
                if (s[i] != '\0') ret += s[i];
            return ret;
        }
    };
    using Index20 = Index<20>;
    using Index30 = Index<30>;
    using Index40 = Index<40>;
    using Usrname = Index20;
    using Pwd = Index30;
    using Mail = Index30;
    template<int len>
    std::istream& operator >> (std::istream &in, Index<len> x) {
        in >> x.s;
        return in;
    }
    template<int len>
    std::ostream& operator << (std::ostream &out, Index<len> x) {
        out << x.s;
        return out;
    }
    
    struct Time {
        int hour, minute;
        Time (int hour = 0, int minute = 0) : hour(hour), minute(minute) {}
        bool operator < (const Time &x) {
            return hour < x.hour || (hour == x.hour && minute < x.minute);
        }
        Time operator + (int x) {
            Time ret(hour, minute + x);
            ret.hour += ret.minute / 60; 
            ret.minute %= 10;
            ret.hour %= 24;
            return ret;
        }
        Time operator - (int x) {
            Time ret(hour, minute - x);
            ret.hour -=
            ret.hour = (ret.hour % 24 + 24) % 24;
            return ret;
        }
    };
    struct Date {

    };
    
} ;
#endif
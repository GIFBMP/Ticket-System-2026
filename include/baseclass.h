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
    using Name = Index<15>;
    using Pwd = Index30;
    using Mail = Index30;
    using TrainID = Index20;
    using Station = Index30;
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
    const int kMinPerHour = 60;
    const int kMinPerDay = 1440;
    const int kInf = 2147483647;
    extern int mon[12];
    int proceedTime(const string&);
    int proceedDate(const string&);
    int getExactMin(int, int, int, int);
    int getDate(int);
    void printTime(int);
    int min(int, int);
    int max(int, int);
} ;
#endif
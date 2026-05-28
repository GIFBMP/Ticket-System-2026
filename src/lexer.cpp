#include "../include/lexer.h"
namespace sjtu {
    vector<string> decompose(const string &s) {
        vector<string> v;
        int len = s.size();
        string str = "";
        for (int i = 0; i < len; i++) {
            if (s[i] == ' ') {
                if (str != "") v.push_back (str);
                str = "";
            }
            else str += s[i];
        }
        if (str != "") v.push_back (str);
        return v;
    }
    vector<string> split(const string &s) {
        vector<string> v;
        int len = s.size();
        string str = "";
        for (int i = 0; i < len; i++) {
            if (s[i] == '|') {
                if (str != "") v.push_back (str);
                str = "";
            }
            else str += s[i];
        }
        if (str != "") v.push_back (str);
        return v;
    } 
    long long stringtoint(const string &s) {
        long long x = 0;
        int len = s.size();
        bool fl = false;
        if (s[0] == '-') fl = true;
        for (int i = 0; i < len; i++)
            if (s[i] >= '0' && s[i] <= '9') {
                x = x * 10 + s[i] - '0';
                //if (x > 2147483647LL) return -1;
            }
        //if (x > 2147483647LL) return -1;
        if (fl) x = -x;
        return x;
    }
}
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
    int stringtoint(const string &s) {
        int x = 0;
        int len = s.size();
        for (int i = 0; i < len; i++) {
            x = x * 10 + s[i] - '0';
        }
        return x;
    }
}
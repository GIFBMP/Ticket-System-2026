#pragma once
#ifndef USER_H
#define USER_H
#include "baseclass.h"
#include "bpt.h"
#include "map.h"
namespace sjtu {
    struct User {
        Usrname username;
        Pwd password;
        Usrname name;
        Mail mailaddr;
        int privilege;
        User() {
            username = Index20("");
            password = Index30("");
            name = Index20("");
            mailaddr = Index30("");
            privilege = 0;
        }
        bool operator < (const User &x) const {
            return username < x.username;
        }
        bool operator <= (const User &x) const {
            return username <= x.username;
        }
        bool operator == (const User &x) const {
            return username == x.username;
        }
    };
    extern bpt<Usrname, User> nameToUser;
    extern map<Usrname, bool> isLogin;
    int add_user(const Usrname&, const Usrname&, const Pwd&, const Usrname&, const Mail&, int);
    int login(const Usrname&, const Pwd&);
    int logout(const Usrname&);
    int query_profile(const Usrname&, const Usrname&);
    int modify_profile(const Usrname&, const Usrname&, const Pwd&, const Usrname&, const Mail&, int);
}

#endif
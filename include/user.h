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
    };
    extern bpt<Usrname, User> nameToUser;
    extern map<Usrname, bool> isLogin;
    int add_user(Usrname, Usrname, Pwd, Usrname, Mail, int);
    int login(Usrname, Pwd);
    int logout(Usrname);
    int query_profile(Usrname, Usrname);
    int modify_profile(Usrname, Usrname, Pwd, Usrname, Mail, int);
}

#endif
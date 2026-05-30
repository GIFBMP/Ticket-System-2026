#include "../include/user.h"
#include "../include/bpt.h"
#include "../include/map.h"
#include <iostream>
namespace sjtu {
    bpt<Usrname, User> nameToUser("Name_to_User_File");
    map<Usrname, bool> isLogin;
    int add_user(const Usrname &cur, const Usrname &usrname, const Pwd &pwd, const Usrname &name, const Mail &mailaddr, int privilege) {
        if (nameToUser.firstadd()) {//first add: ignore cur and privilege
            //std::cout << "firstadd";
            User nw;
            nw.username = usrname;
            nw.password = pwd;
            nw.name = name;
            nw.mailaddr = mailaddr;
            nw.privilege = 10;
            nameToUser.insert(usrname, nw);
            return 0;
        }
        if (!isLogin.count(cur)) {//failed case 1:
            return -1;
        }
        User cur_user = nameToUser.find(cur);
        if (cur_user.privilege <= privilege) {//failed case 2: privilege not enough
            return -1;
        }
        User nw = nameToUser.find(usrname);
        if (!nw.username.empty()) {//failed case 3: already exist
            return -1;
        }
        nw.username = usrname;
        nw.password = pwd;
        nw.name = name;
        nw.mailaddr = mailaddr;
        nw.privilege = privilege;
        nameToUser.insert(usrname, nw);
        return 0;
    }
    int login(const Usrname &username, const Pwd &pwd) {
        if (isLogin.count(username)) return -1;//case 1: user has logined
        User nw = nameToUser.find(username);
        if (nw.username.empty() || pwd != nw.password) return -1;//case 2, 3: user doesn't exist/wrong password
        isLogin[username] = true;
        return 0;
    }
    int logout(const Usrname &username) {
        if (!isLogin.count(username)) return -1;//case 1: user hasn't logined
        // User nw = nameToUser.find(username);
        // if (nw.username.empty()) return -1;//case 2: user doesn't exist
        isLogin.erase(isLogin.find(username));
        return 0;
    }
    int query_profile(const Usrname &cur, const Usrname &x) {
        if (!isLogin.count(cur)) {
            return -1;
        }
        if (x == cur) {
            User nw = nameToUser.find(x);
            std::cout << nw.username << ' ' << nw.name << ' ' << nw.mailaddr << ' ' << nw.privilege << '\n';
            return 0;
        }//self-query
        User cur_user = nameToUser.find(cur);
        User nw = nameToUser.find(x);
        if (nw.username.empty() || cur_user.privilege <= nw.privilege) {
            return -1;
        }
        std::cout << nw.username << ' ' << nw.name << ' ' << nw.mailaddr << ' ' << nw.privilege << '\n';
        return 0;
    }
    int modify_profile(const Usrname &cur, const Usrname &usrname, const Pwd &pwd, const Usrname &name, const Mail &mailaddr, int privilege = -1) {
        if (!isLogin.count(cur)) {
            //std::cerr << "not login\n";
            return -1;
        }
        if (cur == usrname) {
            User nw = nameToUser.find(usrname);
            if (nw.privilege <= privilege) return -1;
            nameToUser.del(usrname, nw);
            if (!pwd.empty()) nw.password = pwd;
            if (!name.empty()) nw.name = name;
            if (!mailaddr.empty()) nw.mailaddr = mailaddr;
            if (privilege >= 0) nw.privilege = privilege;
            nameToUser.insert(usrname, nw);
            std::cout << nw.username << ' ' << nw.name << ' ' << nw.mailaddr << ' ' << nw.privilege << '\n';
            return 0;
        }//self-modify
        User cur_user = nameToUser.find(cur);
        User nw = nameToUser.find(usrname);
        if (cur_user.privilege <= privilege) return -1;
        if (nw.username.empty() || cur_user.privilege <= nw.privilege) {
            // if (nw.username.empty()) std::cerr << "not exist\n";
            // else std::cerr << "not enough privilege\n";
            return -1;
        }
        nameToUser.del(usrname, nw);
        if (!pwd.empty()) nw.password = pwd;
        if (!name.empty()) nw.name = name;
        if (!mailaddr.empty()) nw.mailaddr = mailaddr;
        if (privilege >= 0) nw.privilege = privilege;
        nameToUser.insert(usrname, nw);
        std::cout << nw.username << ' ' << nw.name << ' ' << nw.mailaddr << ' ' << nw.privilege << '\n';
        return 0;
    }
}

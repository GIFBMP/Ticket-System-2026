#include "../include/user.h"
#include "../include/train.h"
#include "../include/ticket.h"
#include "../include/lexer.h"
#include <filesystem>
using namespace sjtu;
void clear() {
    userToTicket.exit();
    tickets.exit();
    trainToPendingTicket.exit();
    ticketToTrain.exit();
    idToTrain.exit();
    stationToID.exit();
    nameToUser.exit();
    if (std::filesystem::exists("User_to_Ticket_File")) std::filesystem::remove("User_to_Ticket_File");
    if (std::filesystem::exists("Tickets_File")) std::filesystem::remove("Tickets_File");
    if (std::filesystem::exists("Train_to_Pending_Ticket_File")) std::filesystem::remove("Train_to_Pending_Ticket_File");
    if (std::filesystem::exists("Ticket_to_Train_File")) std::filesystem::remove("Ticket_to_Train_File");
    if (std::filesystem::exists("ID_to_Train_File")) std::filesystem::remove("ID_to_Train_File");
    if (std::filesystem::exists("Station_to_ID_File")) std::filesystem::remove("Station_to_ID_File");
    if (std::filesystem::exists("Name_to_User_File")) std::filesystem::remove("Name_to_User_File");
    isLogin.clear();
    userToTicket.initialise("User_to_Ticket_File");
    tickets.initialise("Tickets_File");
    trainToPendingTicket.initialise("Train_to_Pending_Ticket_File");
    ticketToTrain.initialise("Ticket_to_Train_File");
    idToTrain.initialise("ID_to_Train_File");
    stationToID.initialise("Station_to_ID_File");
    nameToUser.initialise("Name_to_User_File");
}
int main() {
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(0);
    //std::cout.tie(0);
    clear();
    string s;
    vector<string> v;
    while (getline(std::cin, s)) {
        v = decompose(s);
        int len = v.size();
        if (len <= 1) continue;
        // for (int i = 0; i < len; i++) {
        //     std::cerr << v[i] << ' ';
        // }
        // std::cerr << '\n';
        
        std::cout << v[0] << ' ';
        if (v[1] == "clean") {
            std::cout << "0\n";
            clear();
        }
        else if (v[1] == "exit") {
            std::cout << "bye\n";
            break;
        }
        else if (v[1] == "add_user") {
            //std::cout << "****" << len << '\n';
            Usrname usrname, cur, name;
            Mail mailaddr;
            Pwd pwd;
            int privilege;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-c") cur = v[i + 1];
                if (v[i] == "-u") usrname = v[i + 1];
                if (v[i] == "-n") name = v[i + 1];
                if (v[i] == "-m") mailaddr = v[i + 1];
                if (v[i] == "-p") pwd = v[i + 1];
                if (v[i] == "-g") privilege = stringtoint(v[i + 1]);
            }
            //std::cout << cur << ' ' << usrname << '\n';
            std::cout << add_user(cur, usrname, pwd, name, mailaddr, privilege) << '\n';
        }
        else if (v[1] == "login") {
            Usrname usrname;
            Pwd pwd;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-u") usrname = v[i + 1];
                if (v[i] == "-p") pwd = v[i + 1];
            }
            std::cout << login(usrname, pwd) << '\n';
        }
        else if (v[1] == "logout") {
            std::cout << logout(v[3]) << '\n';
        }
        else if (v[1] == "query_profile") {
            Usrname cur, usrname;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-u") usrname = v[i + 1];
                if (v[i] == "-c") cur = v[i + 1];
            }
            if (query_profile(cur, usrname) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "modify_profile") {
            Pwd pwd = Pwd("");
            Usrname cur, usrname, name = Usrname("");
            Mail mailaddr = Mail("");
            int privilege = -1;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-u") usrname = v[i + 1];
                if (v[i] == "-c") cur = v[i + 1];
                if (v[i] == "-p") pwd = v[i + 1];
                if (v[i] == "-n") name = v[i + 1];
                if (v[i] == "-m") mailaddr = v[i + 1];
                if (v[i] == "-g") privilege = stringtoint(v[i + 1]);
            }
            if (modify_profile(cur, usrname, pwd, name, mailaddr, privilege) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "add_train") {
            std::cout << addtrain(v[3], stringtoint(v[5]), stringtoint(v[7]), v[9], v[11], 
                                  v[13], v[15], v[17], v[19], v[21]) << '\n';
        }
        else if (v[1] == "delete_train") {
            std::cout << deltrain(v[3]) << '\n';
        }
        else if (v[1] == "release_train") {
            std::cout << releasetrain(v[3]) << '\n';
        }
        else if (v[1] == "query_train") {
            if (querytrain(v[3], v[5]) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "query_ticket") {
            int typ = 0;
            if (len >= 9) typ = (v[9] == "cost") ? 1 : 0;
            query_ticket(v[3], v[5], v[7], typ);
        }
        else if (v[1] == "query_transfer") {
            int typ = 0;
            if (len >= 10) typ = (v[9] == "cost") ? 1 : 0;
            query_transfer(v[3], v[5], v[7], typ);
        }
        else if (v[1] == "buy_ticket") {
            bool typ = false;
            if (len >= 16) typ = (v[16] == "true") ? true : false;
            if (buy_ticket(v[3], v[5], v[7], stringtoint(v[9]), v[11], v[13], typ) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "query_order") {
            if (query_order(v[3]) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "refund_ticket") {
            std::cout << refund_ticket(v[3], stringtoint(v[5])) << '\n';
        }
    }
    return 0;
}
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
    dailySeat.exit();
    remainSeats.exit();
    if (std::filesystem::exists("User_to_Ticket_File")) std::filesystem::remove("User_to_Ticket_File");
    if (std::filesystem::exists("Tickets_File")) std::filesystem::remove("Tickets_File");
    if (std::filesystem::exists("Train_to_Pending_Ticket_File")) std::filesystem::remove("Train_to_Pending_Ticket_File");
    if (std::filesystem::exists("Ticket_to_Train_File")) std::filesystem::remove("Ticket_to_Train_File");
    if (std::filesystem::exists("ID_to_Train_File")) std::filesystem::remove("ID_to_Train_File");
    if (std::filesystem::exists("Station_to_ID_File")) std::filesystem::remove("Station_to_ID_File");
    if (std::filesystem::exists("Name_to_User_File")) std::filesystem::remove("Name_to_User_File");
    if (std::filesystem::exists("Daily_Seat_File")) std::filesystem::remove("Daily_Seat_File");
    if (std::filesystem::exists("Remain_Seats_File")) std::filesystem::remove("Remain_Seats_File");
    isLogin.clear();
    userToTicket.initialise("User_to_Ticket_File");
    tickets.initialise("Tickets_File");
    trainToPendingTicket.initialise("Train_to_Pending_Ticket_File");
    ticketToTrain.initialise("Ticket_to_Train_File");
    idToTrain.initialise("ID_to_Train_File");
    stationToID.initialise("Station_to_ID_File");
    nameToUser.initialise("Name_to_User_File");
    dailySeat.initialise("Daily_Seat_File");
    remainSeats.initialise("Remain_Seats_File");
}
int main() {
    //std::ios::sync_with_stdio(false);
    //std::cin.tie(0);
    //std::cout.tie(0);
    // userToTicket.initialise("User_to_Ticket_File");
    // tickets.initialise("Tickets_File");
    // trainToPendingTicket.initialise("Train_to_Pending_Ticket_File");
    // ticketToTrain.initialise("Ticket_to_Train_File");
    // idToTrain.initialise("ID_to_Train_File");
    // stationToID.initialise("Station_to_ID_File");
    // nameToUser.initialise("Name_to_User_File");
    // dailySeat.initialise("Daily_Seat_File");
    // remainSeats.initialise("Remain_Seats_File");
    //clear();
    //clear();
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
            TrainID trainid;
            int stationNum, seatNum;
            string stations, prices, startTime, travelTimes, stopoverTimes, saleDate, typ;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-i") trainid = v[i + 1];
                if (v[i] == "-n") stationNum = stringtoint(v[i + 1]);
                if (v[i] == "-m") seatNum = stringtoint(v[i + 1]);
                if (v[i] == "-s") stations = v[i + 1];
                if (v[i] == "-p") prices = v[i + 1];
                if (v[i] == "-x") startTime = v[i + 1];
                if (v[i] == "-t") travelTimes = v[i + 1];
                if (v[i] == "-d") saleDate = v[i + 1];
                if (v[i] == "-y") typ = v[i + 1];
                if (v[i] == "-o") stopoverTimes = v[i + 1];
            }
            std::cout << addtrain(trainid, stationNum, seatNum, stations, prices, 
                                  startTime, travelTimes, stopoverTimes, saleDate, typ) << '\n';
        }
        else if (v[1] == "delete_train") {
            std::cout << deltrain(v[3]) << '\n';
        }
        else if (v[1] == "release_train") {
            std::cout << releasetrain(v[3]) << '\n';
        }
        else if (v[1] == "query_train") {
            TrainID trainid;
            string date;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-i") trainid = v[i + 1];
                if (v[i] == "-d") date = v[i + 1];
            }
            if (querytrain(date, trainid) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "query_ticket") {
            Station st, ed;
            string date;
            int typ = 0;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-s") st = v[i + 1];
                if (v[i] == "-t") ed = v[i + 1];
                if (v[i] == "-d") date = v[i + 1];
                if (v[i] == "-p") typ = (v[i + 1] == "cost") ? 1 : 0;
            }
            query_ticket(st, ed, date, typ);
        }
        else if (v[1] == "query_transfer") {
            Station st, ed;
            string date;
            int typ = 0;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-s") st = v[i + 1];
                if (v[i] == "-t") ed = v[i + 1];
                if (v[i] == "-d") date = v[i + 1];
                if (v[i] == "-p") typ = (v[i + 1] == "cost") ? 1 : 0;
            }
            query_transfer(st, ed, date, typ);
        }
        else if (v[1] == "buy_ticket") {
            Usrname usrname;
            TrainID trainid;
            string date;
            Station st, ed;
            bool typ = false;
            int num;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-u") usrname = v[i + 1];
                if (v[i] == "-i") trainid = v[i + 1];
                if (v[i] == "-f") st = v[i + 1];
                if (v[i] == "-t") ed = v[i + 1];
                if (v[i] == "-d") date = v[i + 1];
                if (v[i] == "-q") typ = (v[i + 1] == "true") ? true : false;
                if (v[i] == "-n") num = stringtoint(v[i + 1]);
            }
            if (buy_ticket(usrname, trainid, date, num, st, ed, typ) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "query_order") {
            if (query_order(v[3]) == -1) std::cout << "-1\n";
        }
        else if (v[1] == "refund_ticket") {
            Usrname usr;
            int pos;
            for (int i = 2; i < len; i += 2) {
                if (v[i] == "-u") usr = v[i + 1];
                if (v[i] == "-n") pos = stringtoint(v[i + 1]);
            }
            std::cout << refund_ticket(usr, pos) << '\n';
        }
    }
    return 0;
}
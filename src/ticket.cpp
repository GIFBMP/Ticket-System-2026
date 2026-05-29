#include "../include/ticket.h"
namespace sjtu {
    bpt<Usrname, int> userToTicket("User_to_Ticket_File");
    MemoryRiver<Ticket, 0> tickets("Tickets_File");
    bpt<TrainID, int> trainToPendingTicket("Train_to_Pending_Ticket_File");
    MemoryRiver<TrainID, 0> ticketToTrain("Ticket_to_Train_File");
    std::ostream& printTime(std::ostream &out, int total_minute) {
        int day = total_minute / kMinPerDay, hour = 0, month = 1;
        total_minute %= kMinPerDay;
        hour = total_minute % kMinPerHour;
        total_minute %= kMinPerHour;
        day++;
        for (int i = 0; i < 12; i++) {
            if (day > mon[i]) day -= mon[i], month++;
            else break;
        }
        out << month << '-' << day << ' ' << hour << ':' << total_minute;
        return out;
    }
    std::ostream& operator << (std::ostream &out, const Ticket &x) {
        out << x.trainID << ' ' << x.startStation << ' ';
        printTime(out, x.startTime);
        std::cout << " -> ";
        out << x.endStation << ' ';
        printTime(out, x.endTime);
        out << x.ticketCost << ' ' << x.ticketNum;
        return out;
    }
    void query_ticket(Station st, Station ed, const string &date, int typ = 0) {//typ 0:time 1:cost
        vector<TrainID> ret = stationToID.multifind(st);
        Ticket tic;
        tic.startStation = st;
        tic.endStation = ed;
        int Date = proceedDate(date);
        if (typ == 0) {//time
            priority_queue<Ticket, SortByTime> q;
            for (auto x : ret) {
                Train nw = idToTrain.find(x);
                if (nw.is_released == false) continue;
                int st_pos = -1, ed_pos = -1;
                for (int i = 0; i < nw.stationNum; i++) {
                    if (nw.stations[i] == st) st_pos = i;
                    if (nw.stations[i] == ed) ed_pos = i;
                }
                if (ed_pos == -1 || ed_pos < st_pos) continue;
                tic.trainID = x;
                tic.ticketNum = kInf;
                tic.ticketCost = 0;
                int nw_time = nw.startTime;
                for (int i = 0; i < nw.stationNum; i++) {
                    if (i == ed_pos) tic.endTime = nw_time;
                    if (i > 0) nw_time += nw.stopoverTime[i];
                    if (i == st_pos) tic.startTime = nw_time;
                    nw_time += nw.travelTimes[i];
                    if (i >= st_pos && i < ed_pos) {
                        tic.ticketNum = min(tic.ticketNum, nw.seatNum[i]);
                        tic.ticketCost += nw.prices[i];
                    }
                }
                if (getDate(nw.saleStart + tic.startTime) <= Date && Date <= getDate(nw.saleEnd + tic.startTime)) {
                    tic.startTime = tic.startTime % kMinPerDay + Date;
                    tic.endTime = tic.endTime % kMinPerDay + Date;
                    q.push(tic);
                }
            }
            std::cout << q.size() << '\n';
            while (!q.empty()) {
                std::cout << q.top() << '\n';
                q.pop();
            }
        }
        else {
            priority_queue<Ticket, SortByCost> q;
            for (auto x : ret) {
                Train nw = idToTrain.find(x);
                if (nw.is_released == false) continue;
                int st_pos = -1, ed_pos = -1;
                for (int i = 0; i < nw.stationNum; i++) {
                    if (nw.stations[i] == st) st_pos = i;
                    if (nw.stations[i] == ed) ed_pos = i;
                }
                if (ed_pos == -1 || ed_pos < st_pos) continue;
                tic.trainID = x;
                tic.ticketNum = kInf;
                tic.ticketCost = 0;
                int nw_time = nw.startTime;
                for (int i = 0; i < nw.stationNum; i++) {
                    if (i == ed_pos) tic.endTime = nw_time;
                    if (i > 0) nw_time += nw.stopoverTime[i];
                    if (i == st_pos) tic.startTime = nw_time;
                    nw_time += nw.travelTimes[i];
                    if (i >= st_pos && i < ed_pos) {
                        tic.ticketNum = min(tic.ticketNum, nw.seatNum[i]);
                        tic.ticketCost += nw.prices[i];
                    }
                }
                if (getDate(nw.saleStart + tic.startTime) <= Date && Date <= getDate(nw.saleEnd + tic.startTime)) {
                    tic.startTime = tic.startTime % kMinPerDay + Date;
                    tic.endTime = tic.endTime % kMinPerDay + Date;
                    q.push(tic);
                }
            }
            std::cout << q.size() << '\n';
            while (!q.empty()) {
                std::cout << q.top() << '\n';
                q.pop();
            }
        }
    }
    int query_transfer(Station st, Station ed, const string &date, int typ = 0) {//typ 0:time 1:cost
        vector<TrainID> ret = stationToID.multifind(st);
        Ticket ans1, ans2, tmp1, tmp2;
        int total = kInf, total2 = kInf, Date = proceedDate(date);
        for (auto x : ret) {
            Train nw = idToTrain.find(x);
            int st_pos = -1;
            for (int i = 0; i < nw.stationNum; i++) {
                if (nw.stations[i] == st) {
                    st_pos = i;
                    break;
                }
            }
            if (st_pos == nw.stationNum - 1) continue;
            int nw_time = nw.startTime;
            for (int i = 0; i <= st_pos; i++) {
                if (i > 0) nw_time += nw.stopoverTime[i];
                if (i == st_pos) tmp1.startTime = nw_time;
                nw_time += nw.travelTimes[i];
            }
            tmp1.startStation = st;
            tmp1.trainID = x;
            tmp1.ticketNum = nw.seatNum[st_pos];
            tmp1.ticketCost = nw.prices[st_pos];
            if (!(getDate(nw.saleStart + tmp1.startTime) <= Date && Date <= getDate(nw.saleEnd + tmp1.startTime))) continue;
            tmp1.startTime = tmp1.startTime % kMinPerDay + Date;
            for (int i = st_pos + 1; i < nw.stationNum; i++) {
                tmp1.endTime = nw_time % kMinPerDay + Date;
                vector<TrainID> trans = stationToID.multifind(nw.stations[i]);
                tmp2.startStation = tmp1.endStation = nw.stations[i];
                if (nw.stations[i] != ed) {
                    for (auto y : trans) {
                        if (y == x) continue;
                        Train tr_train = idToTrain.find(y);
                        tmp2.trainID = y;
                        tmp2.ticketNum = kInf;
                        tmp2.ticketCost = 0;
                        int tr_pos = -1, ed_pos = -1;
                        for (int j = 0; j < tr_train.stationNum; j++) {
                            if (tr_train.stations[j] == nw.stations[i]) tr_pos = j;
                            if (tr_train.stations[j] == ed) ed_pos = j;
                        }
                        if (ed_pos == -1 || ed_pos <= tr_pos) continue; 
                        int tr_time = 0;
                        for (int j = 0; j < tr_train.stationNum; j++) {
                            if (j == ed_pos) {
                                tmp2.endTime = tr_time;
                                break;
                            }
                            if (j > 0) tr_time += tr_train.stopoverTime[j];
                            if (j == tr_pos) tmp2.startTime = tr_time;
                            tr_time += tr_train.travelTimes[i];
                            if (j >= tr_pos && j < ed_pos) {
                                tmp2.ticketNum = min(tmp2.ticketNum, nw.seatNum[j]);
                                tmp2.ticketCost += nw.prices[j];
                            }
                        }
                        if (tmp1.endTime <= tmp2.startTime + tr_train.saleEnd) {
                            int extra_min = (tmp1.endTime - tmp2.startTime + kMinPerDay - 1) / kMinPerDay * kMinPerDay;
                            tmp2.startTime += max(tr_train.saleStart, extra_min);
                            tmp2.endTime += max(tr_train.saleStart, extra_min);
                            int total_time = tmp2.endTime - tmp1.startTime;
                            int total_cost = tmp2.ticketCost + tmp1.ticketCost;
                            if (typ == 0) {//time
                                if (total_time < total || (total_time == total && total_cost < total2) ||\
                                    (total_time == total && total_cost == total2 && tmp1.trainID < ans1.trainID) ||\
                                    (total_time == total && total_cost == total2 && \
                                     tmp1.trainID == ans1.trainID && tmp2.trainID < ans2.trainID)) {
                                        total = total_time;
                                        total2 = total_cost;
                                        ans1 = tmp1;
                                        ans2 = tmp2;
                                     }
                            }
                            else {//cost
                                if (total_cost < total || (total_cost == total && total_time < total2) ||\
                                    (total_cost == total && total_time == total2 && tmp1.trainID < ans1.trainID) ||\
                                    (total_cost == total && total_time == total2 && \
                                     tmp1.trainID == ans1.trainID && tmp2.trainID < ans2.trainID)) {
                                        total = total_cost;
                                        total2 = total_time;
                                        ans1 = tmp1;
                                        ans2 = tmp2;
                                     }
                            }
                        }
                    }
                }
                tmp1.ticketNum = min(tmp1.ticketNum, nw.seatNum[i]);
                tmp1.ticketCost += nw.prices[i];
                if (i < nw.stationNum - 1) {
                    nw_time += nw.travelTimes[i] + nw.stopoverTime[i];
                }
            }
        }
        if (total == kInf) std::cout << "0\n";
        else std::cout << ans1 << '\n' << ans2 << '\n';
        return 0;
    }
    int buy_ticket(Usrname usr, TrainID id, const string &date, int cnt,
                   Station st, Station ed, bool typ = false) {
        if (!isLogin.count(usr)) return -1;//does not login
        Train nw_train = idToTrain.find(id);
        if (nw_train.trainID.empty() || nw_train.is_released == false) {
            return -1;// train does not exist/has not been released
        }
        int st_pos = -1, ed_pos = -1, Date = proceedDate(date);
        for (int i = 0; i < nw_train.stationNum; i++) {
            if (nw_train.stations[i] == st) st_pos = i;
            if (nw_train.stations[i] == ed) ed_pos = i;
        }
        if (st_pos == -1 || ed_pos == -1 || st_pos > ed_pos) {
            return -1;// route does not exist
        }
        Ticket tic;
        tic.startStation = st;
        tic.endStation = ed;
        tic.trainID = id;
        tic.ticketNum = kInf;
        tic.ticketCost = 0;
        int nw_time = nw_train.startTime;
        for (int i = 0; i < nw_train.stationNum; i++) {
            if (i == ed_pos) tic.endTime = nw_time;
            if (i > 0) nw_time += nw_train.stopoverTime[i];
            if (i == st_pos) tic.startTime = nw_time;
            nw_time += nw_train.travelTimes[i];
            if (i >= st_pos && i < ed_pos) {
                tic.ticketNum = min(tic.ticketNum, nw_train.seatNum[i]);
                tic.ticketCost += nw_train.prices[i];
            }
        }
        if (getDate(nw_train.saleStart + tic.startTime) <= Date && Date <= getDate(nw_train.saleEnd + tic.startTime)\
            && (tic.ticketNum >= cnt || typ == true)) {
            tic.startTime = tic.startTime % kMinPerDay + Date;
            tic.endTime = tic.endTime % kMinPerDay + Date;
            if (typ == false || tic.ticketNum >= cnt) {
                tic.ticketNum = cnt;
                tic.status = TicketStatus::SUCCESS;
                int tic_pos = ticketToTrain.push(id);
                tic.timeStamp = tic_pos;
                tickets.push(tic);
                userToTicket.insert(usr, tic_pos);
                idToTrain.del(id, nw_train);
                for (int i = st_pos; i < ed_pos; i++) {
                    nw_train.seatNum[i] -= tic.ticketNum;
                }
                idToTrain.insert(id, nw_train);
                std::cout << tic.ticketCost << '\n';
            }
            else if (typ == true) {
                tic.ticketNum = cnt;
                tic.status = TicketStatus::PENDING;
                int tic_pos = ticketToTrain.push(id);
                tic.timeStamp = tic_pos;
                tickets.push(tic);
                userToTicket.insert(usr, tic_pos);
                trainToPendingTicket.insert(id, tic_pos);
                std::cout << "queue\n";
            }
            return 0;
        }
        return -1;
    }
    int query_order(Usrname usr) {
        if (!isLogin.count(usr)) return -1;
        vector<int> orders = userToTicket.multifind(usr);
        int len = orders.size();
        std::cout << len << '\n';
        Ticket tmp;
        for (int i = len - 1; ~i; i--) {
            tickets.read(tmp, orders[i]);
            if (tmp.status == TicketStatus::PENDING) std::cout << "[PENDING] ";
            else if (tmp.status == TicketStatus::SUCCESS) std::cout << "[SUCCESS] ";
            else if (tmp.status == TicketStatus::REFUNDED) std::cout << "[REFUNDED] ";
            std::cout << tmp << '\n';
        }
        return 0;
    }
    int refund_ticket(Usrname usr, int ord = 1) {
        if (!isLogin.count(usr)) return -1;
        vector<int> orders = userToTicket.multifind(usr);
        int len = orders.size();
        if (ord > len) return -1;// ticket does not exist
        Ticket tmp;
        int pos = orders[len - ord];
        tickets.read(tmp, pos);
        if (tmp.status == TicketStatus::REFUNDED) return -1; // ticket has been refunded
        else if (tmp.status == TicketStatus::PENDING) {
            TrainID id;
            ticketToTrain.read(id, pos);
            trainToPendingTicket.del(id, pos);
            tmp.status = TicketStatus::REFUNDED;
            tickets.write(tmp, pos);
        }
        else if (tmp.status == TicketStatus::SUCCESS) {
            TrainID id;
            ticketToTrain.read(id, pos);
            Train nw_train = idToTrain.find(id);
            idToTrain.del(id, nw_train);
            int st_pos = -1, ed_pos = -1;
            for (int i = 0; i < nw_train.stationNum; i++) {
                if (nw_train.stations[i] == tmp.startStation) st_pos = i;
                if (nw_train.stations[i] == tmp.startStation) ed_pos = i;
            }
            for (int i = st_pos; i < ed_pos; i++) {
                nw_train.seatNum[i] += tmp.ticketNum;
            }
            vector<int> pending_tickets = trainToPendingTicket.multifind(id);
            int pending_sz = pending_tickets.size();
            for (int i = 0; i < pending_sz; i++) {
                Ticket tic; tickets.read(tic, pending_tickets[i]);
                st_pos = -1, ed_pos = -1;
                for (int j = 0; j < nw_train.stationNum; j++) {
                    if (nw_train.stations[j] == tic.startStation) st_pos = j;
                    if (nw_train.stations[j] == tic.startStation) ed_pos = j;
                }
                int remain_seat = kInf;
                for (int j = st_pos; j < ed_pos; j++) {
                    remain_seat = min (remain_seat, nw_train.seatNum[j]);
                }
                if (tic.ticketNum <= remain_seat) {
                    for (int j = st_pos; j < ed_pos; j++) {
                        nw_train.seatNum[j] -= tic.ticketNum;
                    }
                    tic.status = TicketStatus::SUCCESS;
                    trainToPendingTicket.del(id, pending_tickets[i]);
                    tickets.write(tic, pending_tickets[i]);
                }
            }
            tmp.status = TicketStatus::REFUNDED;
            tickets.write(tmp, pos);
            idToTrain.insert(id, nw_train);
        }
        return 0;
    }
}
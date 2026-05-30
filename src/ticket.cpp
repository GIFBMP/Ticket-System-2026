#include "../include/ticket.h"
namespace sjtu {
    bpt<Usrname, int> userToTicket("User_to_Ticket_File");
    MemoryRiver<Ticket, 0> tickets("Tickets_File");
    bpt<TrainKey, int> trainToPendingTicket("Train_to_Pending_Ticket_File");
    MemoryRiver<TrainID, 0> ticketToTrain("Ticket_to_Train_File");
    std::ostream& printTime(std::ostream &out, int total_minute) {
        int day = total_minute / kMinPerDay, hour = 0, month = 1;
        total_minute %= kMinPerDay;
        hour = total_minute / kMinPerHour;
        total_minute %= kMinPerHour;
        day++;
        for (int i = 0; i < 12; i++) {
            if (day > mon[i]) day -= mon[i], month++;
            else break;
        }
        if (month < 10) out << '0';
        out << month << '-';
        if (day < 10) out << '0';
        out << day << ' ';
        if (hour < 10) out << '0'; 
        out << hour << ':';
        if (total_minute < 10) out << '0';
        out << total_minute;
        return out;
    }
    std::ostream& operator << (std::ostream &out, const Ticket &x) {
        out << x.trainID << ' ' << x.startStation << ' ';
        printTime(out, x.startTime);
        std::cout << " -> ";
        out << x.endStation << ' ';
        printTime(out, x.endTime);
        out << ' ' << x.ticketCost << ' ' << x.ticketNum;
        return out;
    }
    void query_ticket(const Station &st, const Station &ed, const string &date, int typ = 0) {//typ 0:time 1:cost
        int Date = proceedDate(date);
        vector<StartEndVal> ret = ticQry.multifind(StartEndKey{st, ed, Date});
        //std::cout << ret.size() << '\n';
        Ticket tic;
        tic.startStation = st;
        tic.endStation = ed;
        //std::cout << typ << '\n';
        if (typ == 0) {//time
            priority_queue<Ticket, SortByTime> q;
            for (auto x : ret) {
                tic.startTime = x.st_time;
                tic.endTime = x.ed_time;
                tic.ticketCost = x.price;
                tic.trainID = x.id;
                tic.ticketNum = kInf;
                int st_pos = x.st_pos, ed_pos = x.ed_pos;
                RemainSeat seat;
                remainSeats.read(seat, dailySeat.find(TrainKey(x.id, x.departdate)));
                for (int i = st_pos; i < ed_pos; i++) {
                    tic.ticketNum = min(tic.ticketNum, seat.seatNum[i]);
                }
                q.push(tic);
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
                tic.startTime = x.st_time;
                tic.endTime = x.ed_time;
                tic.ticketCost = x.price;
                tic.trainID = x.id;
                tic.ticketNum = kInf;
                int st_pos = x.st_pos, ed_pos = x.ed_pos;
                RemainSeat seat;
                remainSeats.read(seat, dailySeat.find(TrainKey(x.id, x.departdate)));
                for (int i = st_pos; i < ed_pos; i++) {
                    tic.ticketNum = min(tic.ticketNum, seat.seatNum[i]);
                }
                q.push(tic);
            }
            std::cout << q.size() << '\n';
            while (!q.empty()) {
                std::cout << q.top() << '\n';
                q.pop();
            }
        }
    }
    int query_transfer(const Station &st, const Station &ed, const string &date, int typ = 0) {//typ 0:time 1:cost
        //Todo:: reconstruct!
        vector<TrainID> ret = stationToID.multifind(st);
        Ticket ans1, ans2, tmp1, tmp2;
        int total = kInf, total2 = kInf, Date = proceedDate(date);
        map<Station, vector<Ticket> > p;
        for (auto x : ret) {
            Train nw = idToTrain.find(x);
            if (nw.is_released == false) continue;
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
            // if (x == (TrainID)"Thoseofmechanics") {
            //     for (int i = 0; i < nw.stationNum; i++) std::cout << nw.travelTimes[i] << ' ';
            //     std::cout << '\n';
            //     std::cout << nw.startTime << ' ' << tmp1.startTime << '\n';
            // }
            tmp1.startStation = st;
            tmp1.trainID = x;
            tmp1.ticketCost = nw.prices[st_pos];
            int start_ = tmp1.startTime;
            if (!(getDate(nw.saleStart + tmp1.startTime) <= Date && Date <= getDate(nw.saleEnd + tmp1.startTime))) continue;
            int tmp = tmp1.startTime;
            tmp1.startTime = tmp1.startTime % kMinPerDay + Date;
            int start_date = tmp1.startTime - tmp;
            RemainSeat seat;
            remainSeats.read(seat, dailySeat.find(TrainKey(x, start_date)));
            tmp1.ticketNum = seat.seatNum[st_pos];
            for (int i = st_pos + 1; i < nw.stationNum; i++) {
                tmp1.endTime = tmp1.startTime + nw_time - start_;
                tmp1.endStation = nw.stations[i];
                tmp1.date = start_date;
                if (nw.stations[i] != ed) {
                    p[nw.stations[i]].push_back(tmp1);
                }
                tmp1.ticketNum = min(tmp1.ticketNum, seat.seatNum[i]);
                tmp1.ticketCost += nw.prices[i];
                if (i < nw.stationNum - 1) {
                    nw_time += nw.travelTimes[i] + nw.stopoverTime[i];
                }
            }
        }
        ret = stationToID.multifind(ed);
        for (auto x : ret) {
            Train nw = idToTrain.find(x);
            if (nw.is_released == false) continue;
            int ed_pos = -1;
            for (int i = 0; i < nw.stationNum; i++) {
                if (nw.stations[i] == ed) {
                    ed_pos = i;
                    break;
                }
            }
            if (ed_pos == 0) continue;
            int nw_time = nw.startTime, ti[kMaxStation];
            for (int i = 0; i < kMaxStation; i++) ti[i] = 0;
            for (int i = 0; i <= ed_pos; i++) {
                if (i == ed_pos) ti[i] = nw_time; //arrival time
                if (i > 0) nw_time += nw.stopoverTime[i];
                if (i < ed_pos) ti[i] = nw_time;//leave time
                nw_time += nw.travelTimes[i];
            }
            tmp2.ticketCost = 0;
            tmp2.ticketNum = kInf;
            tmp2.trainID = x;
            tmp2.endStation = ed;
            for (int i = ed_pos - 1; ~i; i--) {
                tmp2.startStation = nw.stations[i];
                tmp2.ticketCost += nw.prices[i];
                if (!p.count(nw.stations[i])) continue;
                for (auto tic : p[nw.stations[i]]) {
                    tmp1 = tic; int start_date1 = tmp1.date;
                    if (tmp1.trainID == x) continue;
                    tmp2.startTime = ti[i];
                    tmp2.endTime = ti[ed_pos];
                    tmp2.ticketNum = kInf;
                    if (tmp1.endTime <= tmp2.startTime + nw.saleEnd) {
                        int extra_min = ((tmp1.endTime - tmp2.startTime + kMinPerDay - 1) / kMinPerDay) * kMinPerDay;
                        tmp2.startTime += max(nw.saleStart, extra_min);
                        tmp2.endTime += max(nw.saleStart, extra_min);
                        tmp2.endStation = ed;
                        int start_date2 = max(nw.saleStart, extra_min);
                        RemainSeat seat2;
                        remainSeats.read(seat2, dailySeat.find(TrainKey(x, start_date2)));
                        for (int j = i; j < ed_pos; j++) {
                            tmp2.ticketNum = min(tmp2.ticketNum, seat2.seatNum[j]);
                        }
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
                                    //std::cout << "modified: " << ans1 << ' ' << ans2 << '\n';
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
        }
        if (total == kInf) std::cout << "0\n";
        else std::cout << ans1 << '\n' << ans2 << '\n';
        return 0;
    }
    int buy_ticket(const Usrname &usr, const TrainID &id, const string &date, int cnt,
                   const Station &st, const Station &ed, bool typ = false) {
        if (!isLogin.count(usr)) return -1;//does not login
        Train nw_train = idToTrain.find(id);
        if (nw_train.trainID.empty() || nw_train.is_released == false || cnt > nw_train.seatNum) {
            return -1;// train does not exist/has not been released
        }
        //std::cout << "**()";
        int st_pos = -1, ed_pos = -1, Date = proceedDate(date);
        //std::cout << st << ' ' << ed << '\n';
        for (int i = 0; i < nw_train.stationNum; i++) {
            if (nw_train.stations[i] == st) st_pos = i;
            if (nw_train.stations[i] == ed) ed_pos = i;
        }
        //std::cout << st_pos << ' ' << ed_pos << '\n';
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
                //tic.ticketNum = min(tic.ticketNum, seat.seatNum[i]);
                tic.ticketCost += nw_train.prices[i];
            }
        }
        //std::cout << getDate(nw_train.saleStart + tic.startTime) << ' ' << Date << ' ' << getDate(nw_train.saleEnd + tic.startTime);
        if (!(getDate(nw_train.saleStart + tic.startTime) <= Date && Date <= getDate(nw_train.saleEnd + tic.startTime))) {
            return -1;
        }
        int tmp = tic.startTime;
        int delta = tic.endTime - tic.startTime;
        tic.startTime = tic.startTime % kMinPerDay + Date;
        tic.endTime = tic.startTime + delta;
        int start_date = tic.startTime - tmp;
        RemainSeat seat;
        //std::cout << "start_date:" << start_date << '\n';
        int dayseat_id = dailySeat.find(TrainKey(id, start_date));
        remainSeats.read(seat, dayseat_id);
        for (int i = st_pos; i < ed_pos; i++) {
            tic.ticketNum = min(tic.ticketNum, seat.seatNum[i]);
            //std::cout << seat.seatNum[i] << '\n';
        }
        tic.date = start_date;
        if (tic.ticketNum >= cnt || typ == true) {
            if (typ == false || tic.ticketNum >= cnt) {
                tic.ticketNum = cnt;
                tic.status = TicketStatus::SUCCESS;
                int tic_pos = ticketToTrain.push(const_cast<TrainID&>(id));
                tic.timeStamp = tic_pos;
                tickets.push(tic);
                userToTicket.insert(usr, tic_pos);
                for (int i = st_pos; i < ed_pos; i++) {
                    seat.seatNum[i] -= tic.ticketNum;
                }
                remainSeats.write(seat, dayseat_id);
                std::cout << tic.ticketCost * tic.ticketNum << '\n';
            }
            else if (typ == true) {
                tic.ticketNum = cnt;
                tic.status = TicketStatus::PENDING;
                int tic_pos = ticketToTrain.push(const_cast<TrainID&>(id));
                tic.timeStamp = tic_pos;
                tickets.push(tic);
                userToTicket.insert(usr, tic_pos);
                trainToPendingTicket.insert(TrainKey(id, start_date), tic_pos);
                std::cout << "queue\n";
            }
            return 0;
        }
        return -1;
    }
    int query_order(const Usrname &usr) {
        if (!isLogin.count(usr)) return -1;
        vector<int> orders = userToTicket.multifind(usr);
        int len = orders.size();
        std::cout << len << '\n';
        Ticket tmp;
        for (int i = len - 1; ~i; i--) {
            tickets.read(tmp, orders[i]);
            if (tmp.status == TicketStatus::PENDING) std::cout << "[pending] ";
            else if (tmp.status == TicketStatus::SUCCESS) std::cout << "[success] ";
            else if (tmp.status == TicketStatus::REFUNDED) std::cout << "[refunded] ";
            std::cout << tmp << '\n';
        }
        return 0;
    }
    int refund_ticket(const Usrname &usr, int ord = 1) {
        if (!isLogin.count(usr)) return -1;
        vector<int> orders = userToTicket.multifind(usr);
        int len = orders.size();
        if (ord > len) return -1;// ticket does not exist
        // for (int i = 0; i < len; i++) std::cout << orders[i] << ' ';
        // puts("");
        Ticket tmp;
        int pos = orders[len - ord];
        tickets.read(tmp, pos);
        if (tmp.status == TicketStatus::REFUNDED) return -1; // ticket has been refunded
        else if (tmp.status == TicketStatus::PENDING) {
            TrainID id;
            ticketToTrain.read(id, pos);
            trainToPendingTicket.del(TrainKey(id, tmp.date), pos);
            tmp.status = TicketStatus::REFUNDED;
            tickets.write(tmp, pos);
        }
        else if (tmp.status == TicketStatus::SUCCESS) {
            TrainID id;
            ticketToTrain.read(id, pos);
            Train nw_train = idToTrain.find(id);
            //idToTrain.del(id, nw_train);
            int st_pos = -1, ed_pos = -1;
            for (int i = 0; i < nw_train.stationNum; i++) {
                if (nw_train.stations[i] == tmp.startStation) st_pos = i;
                if (nw_train.stations[i] == tmp.endStation) ed_pos = i;
            }
            RemainSeat seat;
            int dayseat_id = dailySeat.find(TrainKey(id, tmp.date));
            // printTime(tmp.date); std::cout << "???\n";
            // std::cout << tmp.startStation << ' ' << tmp.endStation << '\n';
            remainSeats.read(seat, dayseat_id);
            for (int i = st_pos; i < ed_pos; i++) {
                seat.seatNum[i] += tmp.ticketNum;
            }
            vector<int> pending_tickets = trainToPendingTicket.multifind(TrainKey(id, tmp.date));
            int pending_sz = pending_tickets.size();
            for (int i = 0; i < pending_sz; i++) {
                Ticket tic; tickets.read(tic, pending_tickets[i]);
                st_pos = -1, ed_pos = -1;
                for (int j = 0; j < nw_train.stationNum; j++) {
                    if (nw_train.stations[j] == tic.startStation) st_pos = j;
                    if (nw_train.stations[j] == tic.endStation) ed_pos = j;
                }
                int remain_seat = kInf;
                for (int j = st_pos; j < ed_pos; j++) {
                    remain_seat = min (remain_seat, seat.seatNum[j]);
                }
                if (tic.ticketNum <= remain_seat) {
                    for (int j = st_pos; j < ed_pos; j++) {
                        seat.seatNum[j] -= tic.ticketNum;
                    }
                    tic.status = TicketStatus::SUCCESS;
                    trainToPendingTicket.del(TrainKey(id, tmp.date), pending_tickets[i]);
                    tickets.write(tic, pending_tickets[i]);
                }
            }
            tmp.status = TicketStatus::REFUNDED;
            tickets.write(tmp, pos);
            remainSeats.write(seat, dayseat_id);
            //idToTrain.insert(id, nw_train);
        }
        return 0;
    }
}
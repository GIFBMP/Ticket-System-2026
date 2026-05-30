#pragma once
#ifndef TICKET_H
#define TICKET_H
#include "train.h"
#include "user.h"
#include "priority_queue.h"
namespace sjtu {
    enum TicketStatus{SUCCESS, PENDING, REFUNDED};
    struct Ticket {
        Station startStation, endStation;
        TrainID trainID;
        int startTime, endTime, timeStamp, ticketNum, ticketCost, date;
        TicketStatus status;
        Ticket() {
            startStation = endStation = Station("");
            trainID = TrainID("");
            startTime = endTime = timeStamp = ticketNum = 0;
            status = TicketStatus::PENDING;
            date = 0;
        }
        bool operator < (const Ticket &other) {
            return timeStamp < other.timeStamp;
        }
        bool operator <= (const Ticket &other) {
            return timeStamp <= other.timeStamp;
        }
        bool operator == (const Ticket &other) {
            return timeStamp == other.timeStamp;
        }
    };
    struct SortByTime {
        bool operator () (const Ticket &x, const Ticket &y) {
            if (x.endTime - x.startTime != y.endTime - y.startTime) {
                return x.endTime - x.startTime > y.endTime - y.startTime;
            }
            return y.trainID < x.trainID;
        }
    };
    struct SortByCost {
        bool operator () (const Ticket &x, const Ticket &y) {
            if (x.ticketCost != y.ticketCost) {
                return x.ticketCost > y.ticketCost;
            }
            return y.trainID < x.trainID;
        }
    };
    
    std::ostream& operator << (std::ostream &, const Ticket &);
    void query_ticket(const Station&, const Station&, const string &, int);//0:time 1:cost
    int query_transfer(const Station&, const Station&, const string &, int);
    int buy_ticket(const Usrname&, const TrainID&, const string&, int, const Station&, const Station&, bool);
    int query_order(const Usrname&);
    int refund_ticket(const Usrname&, int);
    extern bpt<Usrname, int> userToTicket;
    extern bpt<TrainKey, int> trainToPendingTicket;
    extern MemoryRiver<Ticket, 0> tickets;
    extern MemoryRiver<TrainID, 0> ticketToTrain;
}
#endif
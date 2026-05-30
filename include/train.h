#pragma once
#ifndef TRAIN_H
#define TRAIN_H
#include "baseclass.h"
#include "bpt.h"
namespace sjtu {
    const int kMaxStation = 26;
    struct Train {
        TrainID trainID;
        int stationNum;
        Station stations[kMaxStation];
        int prices[kMaxStation];
        int startTime;
        int travelTimes[kMaxStation], stopoverTime[kMaxStation];
        bool is_released;
        int saleStart, saleEnd;
        char typ;
        Train() {
            trainID = Index20("");
            stationNum = 0;
            for (int i = 0; i < kMaxStation; i++) {
                stations[i] = Index30("");
                travelTimes[i] = stopoverTime[i] = 0;
            }
            startTime = 0;
            is_released = false;
            typ = '\0';
        }
        bool operator < (const Train &x) const {
            return trainID < x.trainID;
        }
        bool operator <= (const Train &x) const {
            return trainID <= x.trainID;
        }
        bool operator == (const Train &x) const {
            return trainID == x.trainID;
        }
    };
    struct TrainKey {
        TrainID id;
        int date;
        TrainKey (TrainID x = TrainID(""), int y = 0) : id(x), date(y) {}
        bool operator < (const TrainKey &x) const {
            return id == x.id ? date < x.date : id < x.id;
        }
        bool operator <= (const TrainKey &x) const {
            return id == x.id ? date <= x.date : id < x.id;
        }
        bool operator == (const TrainKey &x) const {
            return id == x.id && date == x.date;
        }
    };
    struct RemainSeat {
        int seatNum[kMaxStation];
        RemainSeat () {
            for (int i = 0; i < kMaxStation; i++) {
                seatNum[i] = 0;
            }
        }

    };
    int addtrain(const TrainID&, int, int, const string&, const string&, const string&, const string&, \
                 const string&, const string&, const string&);
    int deltrain(const TrainID&);
    int releasetrain(const TrainID&);
    int querytrain(const string&, const TrainID&);
    extern bpt<TrainID, Train> idToTrain;
    extern bpt<Station, TrainID> stationToID;
    extern bpt<TrainKey, int> dailySeat;
    extern MemoryRiver<RemainSeat, 0> remainSeats; 
}
#endif
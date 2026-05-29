#pragma once
#ifndef TRAIN_H
#define TRAIN_H
#include "baseclass.h"
#include "bpt.h"
namespace sjtu {
    const int kMaxStation = 100;
    struct Train {
        TrainID trainID;
        int stationNum;
        Station stations[kMaxStation];
        int seatNum[kMaxStation];
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
                seatNum[i] = 0;
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
    int addtrain(TrainID, int, int, const string&, const string&, const string&, const string&, \
                 const string&, const string&, const string&);
    int deltrain(TrainID);
    int releasetrain(TrainID);
    int querytrain(const string&, TrainID);
    extern bpt<TrainID, Train> idToTrain;
    extern bpt<Station, TrainID> stationToID;
}
#endif
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
    };
    class TrainKey {
        Station start_station, end_station;
        TrainKey() {
            start_station = Index30("");
            end_station = Index30("");
        }
    };
    int addtrain(TrainID, int, int, string, string, string, string, string, string, string);
    int deltrain();
    int releasetrain();
    int querytrain();
    int query_ticket();
    int query_transfer();
    extern bpt<Index20, Train> idToTrain;
    extern bpt<Station, Index20> stationToID;
}
#endif
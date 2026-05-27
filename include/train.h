#pragma once
#ifndef TRAIN_H
#define TRAIN_H
#include "baseclass.h"
namespace sjtu {
    const int kMaxStation = 100;
    class Train {
        Index20 trainID;
        int stationNum;
        Index30 stations[kMaxStation];
        int seatNum;
        int prices[kMaxStation];
        Time startTime;
        int travelTimes[kMaxStation], stopoverTime[kMaxStation];
        
    };
    class TrainKey {

    };
}
#endif
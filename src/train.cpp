#include "../include/train.h"
#include "../include/lexer.h"
namespace sjtu {
    bpt<Index20, Train> idToTrain;
    bpt<Station, Index20> stationToID;
    int addtrain(TrainID trainID, int stationNum, int seatNum, string stations, string prices, \
                 string startTime, string travelTimes, string stopoverTimes, string saleDate, string typ) {
        Train nw = idToTrain.find(trainID);
        if (!nw.trainID.empty()) return -1;
        nw.trainID = trainID;
        nw.stationNum = stationNum;
        vector<string> stationName = split(stations), Prices = split(prices);
        for (int i = 0; i < stationNum; i++) {
            nw.stations[i] = stationName[i];
            if (i < stationNum - 1) {
                nw.seatNum[i] = seatNum;
                nw.prices[i] = stringtoint(Prices[i]);
            }
        }
        nw.typ = typ[0];
        nw.startTime = proceedTime(startTime);
        nw.saleStart = proceedDate(saleDate.substr(0, 5));
        nw.saleEnd = proceedDate(saleDate.substr(6, 5));
        nw.is_released = false;
        idToTrain.insert(trainID, nw);
        for (int i = 0; i < stationNum; i++) {
            stationToID.insert(stations[i] , trainID);
        }
        return 0;
    }
    int deltrain() {
        //TODO
        return 0;
    }
}
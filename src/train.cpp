#include "../include/train.h"
#include "../include/lexer.h"
namespace sjtu {
    bpt<TrainID, Train> idToTrain("ID_to_Train_File");
    bpt<Station, TrainID> stationToID("Station_to_ID_File");
    int addtrain(TrainID trainID, int stationNum, int seatNum, const string& stations, const string& prices, \
                 const string& startTime, const string& travelTimes, const string& stopoverTimes, const string& saleDate, const string& typ) {
        Train nw = idToTrain.find(trainID);
        if (!nw.trainID.empty()) return -1;
        nw.trainID = trainID;
        nw.stationNum = stationNum;
        vector<string> stationName = split(stations), Prices = split(prices), stopover = split(stopoverTimes);
        for (int i = 0; i < stationNum; i++) {
            nw.stations[i] = stationName[i];
            if (i < stationNum - 1) {
                nw.seatNum[i] = seatNum;
                nw.prices[i] = stringtoint(Prices[i]);
                if (i > 0) {
                    nw.stopoverTime[i] = stringtoint(stopover[i - 1]);
                }
            }
        }
        nw.typ = typ[0];
        nw.startTime = proceedTime(startTime);
        nw.saleStart = proceedDate(saleDate.substr(0, 5));
        nw.saleEnd = proceedDate(saleDate.substr(6, 5));
        nw.is_released = false;
        idToTrain.insert(trainID, nw);
        for (int i = 0; i < stationNum; i++) {
            stationToID.insert(stationName[i], trainID);
        }
        return 0;
    }
    int deltrain(TrainID id) {
        Train nw = idToTrain.find(id);
        if (nw.trainID.empty()) {//does not exist
            return -1; 
        }
        if (nw.is_released == true) { //has been released
            return -1;
        }
        idToTrain.del(id, nw);
        for (int i = 0; i < nw.stationNum; i++) {
            stationToID.del(nw.stations[i], id);
        }
        return 0;
    }
    int releasetrain(TrainID id) {
        Train nw = idToTrain.find(id);
        if (nw.trainID.empty()) {//does not exist
            return -1; 
        }
        if (nw.is_released == true) { //has been released
            return -1;
        }
        idToTrain.del(id, nw);
        nw.is_released = true;
        idToTrain.insert(id, nw);
        return 0;
    }
    int querytrain(const string &date, TrainID id) {
        Train nw = idToTrain.find(id);
        if (nw.trainID.empty()) return -1;
        int Date = proceedDate(date);
        if (Date < nw.saleStart || Date > nw.saleEnd) return -1;
        int beginTime = Date + nw.startTime, totalprice = 0;
        for (int i = 0; i < nw.stationNum; i++) {
            std::cout << nw.stations[i] << ' ';
            if (i == 0) std::cout << "xx-xx xx:xx";
            else printTime(Date);
            std::cout << " -> ";
            if (i > 0) Date += nw.stopoverTime[i];
            if (i == nw.stationNum - 1) std::cout << "xx-xx xx:xx";
            else printTime(Date);
            std::cout << ' ' << totalprice << ' ';
            if (i == nw.stationNum - 1) std::cout << "x\n";
            else std::cout << nw.seatNum[i];
            Date += nw.travelTimes[i];
            totalprice += nw.prices[i];
        }
        return 0;
    }
}
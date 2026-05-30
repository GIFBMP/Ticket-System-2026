#include "../include/train.h"
#include "../include/lexer.h"
namespace sjtu {
    bpt<TrainID, Train> idToTrain("ID_to_Train_File");
    bpt<Station, TrainID> stationToID("Station_to_ID_File");
    bpt<TrainKey, int> dailySeat("Daily_Seat_File");
    MemoryRiver<RemainSeat, 0> remainSeats("Remain_Seats_File");
    bpt<StartKey, TrainID> startToTrain("Start_to_Train_File");
    int addtrain(const TrainID &trainID, int stationNum, int seatNum, const string& stations, const string& prices, \
                 const string& startTime, const string& travelTimes, const string& stopoverTimes, const string& saleDate, const string& typ) {
        Train nw = idToTrain.find(trainID);
        if (!nw.trainID.empty()) return -1;
        nw.trainID = trainID;
        nw.stationNum = stationNum;
        vector<string> stationName = split(stations), Prices = split(prices), 
                       stopover = split(stopoverTimes), travel = split(travelTimes);
        //std::cout << stopoverTimes << '\n';
        //std::cout << "(((())))\n" << stationName.size() << ' ' << Prices.size() << ' ' << stopover.size() << '\n';
        RemainSeat seat;
        for (int i = 0; i < stationNum; i++) {
            nw.stations[i] = stationName[i];
            if (i < stationNum - 1) {
                seat.seatNum[i] = seatNum;
                nw.prices[i] = stringtoint(Prices[i]);
                nw.travelTimes[i] = stringtoint(travel[i]);
                if (i > 0) {
                    nw.stopoverTime[i] = stringtoint(stopover[i - 1]);
                }
            }
        }
        
        //std::cout << "(((())))\n";
        nw.seatNum = seatNum;
        nw.typ = typ[0];
        nw.startTime = proceedTime(startTime);
        nw.saleStart = proceedDate(saleDate.substr(0, 5));
        nw.saleEnd = proceedDate(saleDate.substr(6, 5));
        nw.is_released = false;
        idToTrain.insert(trainID, nw);
        for (int i = nw.saleStart; i <= nw.saleEnd; i += kMinPerDay) {
            //std::cout << "day:" << i << ' ';
            int cnt = remainSeats.push(seat);
            dailySeat.insert(TrainKey(trainID, i), cnt);
        }
        return 0;
    }
    int deltrain(const TrainID &id) {
        Train nw = idToTrain.find(id);
        if (nw.trainID.empty()) {//does not exist
            return -1; 
        }
        if (nw.is_released == true) { //has been released
            return -1;
        }
        for (int i = nw.saleStart; i <= nw.saleEnd; i += kMinPerDay) {
            int cnt = dailySeat.find(TrainKey(id, i));
            dailySeat.del(TrainKey(id, i), cnt);
        }
        idToTrain.del(id, nw);
        return 0;
    }
    int releasetrain(const TrainID &id) {
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
        for (int i = 0; i < nw.stationNum; i++) {
            stationToID.insert(nw.stations[i], id);
        }
        int nw_time = nw.startTime, lev[kMaxStation] = {0};
        for (int i = 0; i < nw.stationNum; i++) {
            if (i > 0 && i < nw.stationNum - 1) nw_time += nw.stopoverTime[i];
            lev[i] = nw_time;
            nw_time += nw.travelTimes[i];
        }
        for (int ti = nw.saleStart; ti <= nw.saleEnd; ti += kMinPerDay) {
            for (int i = 0; i < nw.stationNum - 1; i++) {
                int pri = nw.prices[i];
                StartKey key;
                key.date = getDate(lev[i] + ti);
                key.st = nw.stations[i];
                startToTrain.insert(key, id);
            }
        }
        return 0;
    }
    int querytrain(const string &date, const TrainID &id) {
        Train nw = idToTrain.find(id);
        if (nw.trainID.empty()) return -1;
        int Date = proceedDate(date);
        if (Date < nw.saleStart || Date > nw.saleEnd) return -1;
        int beginTime = Date + nw.startTime, totalprice = 0;
        RemainSeat seat;
        remainSeats.read(seat, dailySeat.find(TrainKey(id, Date)));
        std::cout << id << ' ' << nw.typ << '\n';
        for (int i = 0; i < nw.stationNum; i++) {
            std::cout << nw.stations[i] << ' ';
            if (i == 0) std::cout << "xx-xx xx:xx";
            else printTime(beginTime);
            std::cout << " -> ";
            if (i > 0) beginTime += nw.stopoverTime[i];
            if (i == nw.stationNum - 1) std::cout << "xx-xx xx:xx";
            else printTime(beginTime);
            std::cout << ' ' << totalprice << ' ';
            if (i == nw.stationNum - 1) std::cout << "x\n";
            else std::cout << seat.seatNum[i] << '\n';
            beginTime += nw.travelTimes[i];
            totalprice += nw.prices[i];
        }
        return 0;
    }
}
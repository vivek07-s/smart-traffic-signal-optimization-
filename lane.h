#ifndef LANE_H
#define LANE_H

#include "vehicle.h"
#include <queue>
#include <string>
#include <vector>
using namespace std;

class Lane {
private:
    string laneName;
    queue<Vehicle> vehicleQueue;
    int totalVehicles;
    int totalWaitingTime;
    int capacity;
    bool blocked;
    double blockagePercent;
    
public:
    Lane(string name, int cap = 50);
    
    void addVehicle(const Vehicle& v);
    Vehicle removeVehicle();
    int getVehicleCount() const;
    
    bool hasEmergencyVehicle() const;
    
    void updateAllWaitingTimes(int currentTime);
    int getTotalWaitingTime() const;
    int getAverageWaitingTime() const;
    
    double calculatePriority() const;
    
    void setBlockage(double percent);
    bool isBlocked() const;
    int getEffectiveCapacity() const;
    
    string getLaneName() const;
    void displayLaneStatus() const;
    bool isEmpty() const;
    bool isFull() const;
};

#endif

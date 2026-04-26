#ifndef SIMULATION_H
#define SIMULATION_H

#include "lane.h"
#include "trafficSignal.h"
#include "priorityQueue.h"
#include "trafficPatterns.h"
#include "visualisation.h"
#include <vector>
#include <map>
using namespace std;

class Simulation {
private:
    vector<Lane*> lanes;
    TrafficSignal signal;
    PriorityQueue priorityQueue;
    TrafficPattern trafficPattern;
    
    int currentTime;
    int totalCycles;
    int vehicleIDCounter;
    
    int totalVehiclesProcessed;
    int totalVehiclesGenerated;
    int totalWaitingTime;
    double avgWaitingTime;
    
    void generateVehicles();
    Lane* selectNextLane();
    void updateWaitingTimes();
    
public:
    Simulation();
    ~Simulation();
    
    void initializeLanes();
    void setSignalTimings(int minGreen, int maxGreen, int yellow);
    void setTrafficPattern(TimeOfDay period);
    
    void addEmergencyVehicle(string laneName);
    void blockLane(string laneName, double percentage);
    
    void runCycle();
    void runSimulation(int numCycles);
    
    void displayStatistics() const;
    void displayAllLanes() const;
    double getAverageWaitingTime() const;
    int getTotalProcessed() const;
    
    void analyzeComplexity() const;
    
    Lane* getLane(int index);
    Lane* getLane(string name);
    
    void runFixedCycle(Lane* lane, int duration);
};

#endif

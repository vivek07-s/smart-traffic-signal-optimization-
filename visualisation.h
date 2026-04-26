#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "lane.h"
#include <string>
#include <vector>
using namespace std;

class TrafficVisualizer {
public:
    static void displayIntersection(
        Lane* north, Lane* south, Lane* east, Lane* west,
        Lane* currentGreen
    );
    
    static void displayCompactStatus(
        vector<Lane*> lanes,
        Lane* currentGreen,
        int cycleNumber
    );
    
    static void displayWaitingBar(int waitTime, int maxWait = 120);
    
private:
    static string getSignalSymbol(Lane* lane, Lane* greenLane);
    static string getVehicleSymbol(int count);
};

#endif

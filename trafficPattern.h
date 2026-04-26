#ifndef TRAFFIC_PATTERNS_H
#define TRAFFIC_PATTERNS_H

#include <string>
#include <cstdlib>
using namespace std;

enum TimeOfDay {
    MORNING_RUSH,
    MIDDAY,
    EVENING_RUSH,
    NIGHT
};

class TrafficPattern {
private:
    TimeOfDay currentPeriod;
    
public:
    TrafficPattern();
    TrafficPattern(TimeOfDay period);
    
    int getVehicleRate(string laneName) const;
    
    double getEmergencyProbability() const;
    
    void setPeriod(TimeOfDay period);
    TimeOfDay getCurrentPeriod() const;
    
    void displayPattern() const;
    string getPeriodName() const;
};

#endif

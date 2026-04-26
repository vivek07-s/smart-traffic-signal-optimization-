#include "trafficPatterns.h"
#include <iostream>
using namespace std;

TrafficPattern::TrafficPattern() {
    currentPeriod = MIDDAY;
}

TrafficPattern::TrafficPattern(TimeOfDay period) {
    currentPeriod = period;
}

int TrafficPattern::getVehicleRate(string laneName) const {
    if (currentPeriod == MORNING_RUSH) {
        if (laneName == "North") return rand() % 3 + 4;
        if (laneName == "East") return rand() % 3 + 4;
        if (laneName == "South") return rand() % 2 + 1;
        if (laneName == "West") return rand() % 2 + 1;
    }
    else if (currentPeriod == EVENING_RUSH) {
        if (laneName == "North") return rand() % 2 + 1;
        if (laneName == "East") return rand() % 2 + 1;
        if (laneName == "South") return rand() % 3 + 4;
        if (laneName == "West") return rand() % 3 + 4;
    }
    else if (currentPeriod == MIDDAY) {
        return rand() % 3 + 2;
    }
    else if (currentPeriod == NIGHT) {
        return rand() % 2;
    }
    
    return rand() % 3;
}

double TrafficPattern::getEmergencyProbability() const {
    if (currentPeriod == MORNING_RUSH || currentPeriod == EVENING_RUSH) {
        return 0.03;
    }
    return 0.01;
}

void TrafficPattern::setPeriod(TimeOfDay period) {
    currentPeriod = period;
}

TimeOfDay TrafficPattern::getCurrentPeriod() const {
    return currentPeriod;
}

string TrafficPattern::getPeriodName() const {
    switch(currentPeriod) {
        case MORNING_RUSH: return "Morning Rush Hour (7-9 AM)";
        case MIDDAY: return "Midday Regular Traffic (12-2 PM)";
        case EVENING_RUSH: return "Evening Rush Hour (5-7 PM)";
        case NIGHT: return "Night Time (10 PM-6 AM)";
        default: return "Unknown";
    }
}

void TrafficPattern::displayPattern() const {
    cout << "\nCurrent Traffic Pattern: " << getPeriodName() << endl;
    
    cout << "\nExpected Traffic Density:" << endl;
    cout << "  North: ";
    if (currentPeriod == MORNING_RUSH) cout << "HIGH";
    else if (currentPeriod == EVENING_RUSH) cout << "LOW";
    else if (currentPeriod == MIDDAY) cout << "MEDIUM";
    else cout << "VERY LOW";
    cout << endl;
    
    cout << "  East:  ";
    if (currentPeriod == MORNING_RUSH) cout << "HIGH";
    else if (currentPeriod == EVENING_RUSH) cout << "LOW";
    else if (currentPeriod == MIDDAY) cout << "MEDIUM";
    else cout << "VERY LOW";
    cout << endl;
    
    cout << "  South: ";
    if (currentPeriod == EVENING_RUSH) cout << "HIGH";
    else if (currentPeriod == MORNING_RUSH) cout << "LOW";
    else if (currentPeriod == MIDDAY) cout << "MEDIUM";
    else cout << "VERY LOW";
    cout << endl;
    
    cout << "  West:  ";
    if (currentPeriod == EVENING_RUSH) cout << "HIGH";
    else if (currentPeriod == MORNING_RUSH) cout << "LOW";
    else if (currentPeriod == MIDDAY) cout << "MEDIUM";
    else cout << "VERY LOW";
    cout << endl;
}

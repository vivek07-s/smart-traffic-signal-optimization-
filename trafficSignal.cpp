#include "trafficSignal.h"
#include <iostream>
#include <iomanip>
using namespace std;

TrafficSignal::TrafficSignal() {
    currentLane = nullptr;
    currentState = RED;
    minGreenDuration = 15;
    maxGreenDuration = 60;
    yellowDuration = 5;
    redDuration = 2;
    elapsedTime = 0;
}

TrafficSignal::TrafficSignal(int minGreen, int maxGreen, int yellow) {
    currentLane = nullptr;
    currentState = RED;
    minGreenDuration = minGreen;
    maxGreenDuration = maxGreen;
    yellowDuration = yellow;
    redDuration = 2;
    elapsedTime = 0;
}

string TrafficSignal::getStateName(SignalState state) const {
    switch(state) {
        case GREEN: return "GREEN";
        case YELLOW: return "YELLOW";
        case RED: return "RED";
        default: return "UNKNOWN";
    }
}

string TrafficSignal::getSignalSymbol() const {
    switch(currentState) {
        case GREEN: return "🟢";
        case YELLOW: return "🟡";
        case RED: return "🔴";
        default: return "⚪";
    }
}

// ADAPTIVE TIMING: Calculate optimal green time based on queue
int TrafficSignal::calculateOptimalGreenTime(Lane* lane) const {
    if (!lane) return minGreenDuration;
    
    int vehicleCount = lane->getVehicleCount();
    
    // Formula: 10 seconds base + 2 seconds per vehicle
    // Assumption: 1 vehicle passes every 2 seconds
    int optimalTime = 10 + (vehicleCount * 2);
    
    // Clamp between min and max
    if (optimalTime < minGreenDuration) return minGreenDuration;
    if (optimalTime > maxGreenDuration) return maxGreenDuration;
    
    return optimalTime;
}

void TrafficSignal::setGreenSignal(Lane* lane, int duration) {
    currentLane = lane;
    currentState = GREEN;
    elapsedTime = 0;
    
    cout << "\n🟢 GREEN Signal: " << lane->getLaneName() 
         << " (Duration: " << duration << "s)" << endl;
}

void TrafficSignal::setYellowSignal() {
    currentState = YELLOW;
    elapsedTime = 0;
    cout << "🟡 YELLOW Signal" << endl;
}

void TrafficSignal::setRedSignal() {
    currentState = RED;
    elapsedTime = 0;
    cout << "🔴 RED Signal" << endl;
}

void TrafficSignal::updateSignal(int timeStep) {
    elapsedTime += timeStep;
}

SignalState TrafficSignal::getCurrentState() const {
    return currentState;
}

Lane* TrafficSignal::getCurrentLane() const {
    return currentLane;
}

int TrafficSignal::getMinGreenDuration() const {
    return minGreenDuration;
}

int TrafficSignal::getMaxGreenDuration() const {
    return maxGreenDuration;
}

int TrafficSignal::getElapsedTime() const {
    return elapsedTime;
}

// Clear vehicles from lane during green signal
int TrafficSignal::clearVehicles(Lane* lane, int duration) {
    if (!lane) return 0;
    
    // Assumption: 1 vehicle passes every 2 seconds
    int maxVehicles = duration / 2;
    int vehiclesCleared = 0;
    
    cout << "  🚗 Clearing vehicles..." << endl;
    
    for (int i = 0; i < maxVehicles && !lane->isEmpty(); i++) {
        Vehicle v = lane->removeVehicle();
        vehiclesCleared++;
        
        // Show emergency vehicles being cleared
        if (v.isEmergency()) {
            cout << "  🚨 Emergency vehicle cleared!" << endl;
        }
    }
    
    cout << "  ✅ Cleared " << vehiclesCleared << " vehicles from " 
         << lane->getLaneName() << endl;
    cout << "  ⏳ " << lane->getVehicleCount() << " vehicles still waiting" << endl;
    
    return vehiclesCleared;
}

void TrafficSignal::displaySignalStatus() const {
    cout << "\n┌─────────────────────────────────────┐" << endl;
    cout << "│    TRAFFIC SIGNAL STATUS            │" << endl;
    cout << "├─────────────────────────────────────┤" << endl;
    
    if (currentLane) {
        cout << "│ Lane:     " << setw(24) << left << currentLane->getLaneName() << "│" << endl;
    } else {
        cout << "│ Lane:     " << setw(24) << left << "None" << "│" << endl;
    }
    
    cout << "│ State:    " << setw(24) << left << (getSignalSymbol() + " " + getStateName(currentState)) << "│" << endl;
    cout << "│ Elapsed:  " << setw(24) << left << (to_string(elapsedTime) + "s") << "│" << endl;
    cout << "└─────────────────────────────────────┘\n" << endl;
}

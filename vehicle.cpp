#include "vehicle.h"
#include <bits/stdc++.h>
using namespace std;

Vehicle::Vehicle(int id, VehicleType vType)
{
    vehicleID = id;
    arrivalTime = time(0);
    waitingTime = 0;
    type = vType;
}

Vehicle::Vehicle(int id, time_t arrival, VehicleType vType)
{
    vehicleID = id;
    arrivalTime = arrival;
    waitingTime = 0;
    type = vType;
}

int Vehicle::getVehicleID() const
{
    return vehicleID;
}

time_t Vehicle::getArrivalTime() const
{
    return arrivalTime;
}

int Vehicle::getWaitingTime() const
{
    return waitingTime;
}

VehicleType Vehicle::getType() const
{
    return type;
}

bool Vehicle::isEmergency() const
{
    return type == EMERGENCY;
}

bool Vehicle::isPublicTransport() const
{
    return type == PUBLIC_TRANSPORT;
}

int Vehicle::getPriorityMultiplier() const
{
    if (type == EMERGENCY)
        return 10;
    else if (type == PUBLIC_TRANSPORT)
        return 2;
    else
        return 1;
}

void Vehicle::updateWaitingTime(int currentTime)
{
    waitingTime = currentTime - arrivalTime;
}

string Vehicle::getTypeString() const 
{
    switch(type) {
        case EMERGENCY: 
            return "EMERGENCY";
        case PUBLIC_TRANSPORT: 
            return "BUS";
        default: 
            return "REGULAR";
    }
}

void Vehicle::displayVehicle() const
{
    cout << "Vehicle ID: " << vehicleID << ", Type: " << getTypeString() 
         << ", Arrival Time: " << ctime(&arrivalTime) 
         << ", Waiting Time: " << waitingTime << " seconds" << endl;
}

#include "lane.h"
#include <bits/stdc++.h>
using namespace std;

Lane::Lane(string name, int cap)
{
    laneName = name;
    capacity = cap;
    totalVehicles = 0;
    totalWaitingTime = 0;
    blocked = false;
    blockagePercent = 0.0;
}

void Lane::addVehicle(const Vehicle& v)
{
    if (isFull())
    {
        cout << "Lane " << laneName << " is full. Cannot add vehicle " << v.getVehicleID() << endl;
        return;
    }
    vehicleQueue.push(v);
    totalVehicles++;
}

Vehicle Lane::removeVehicle()
{
    if(!isEmpty())
    {
        Vehicle v = vehicleQueue.front();
        vehicleQueue.pop();
        totalVehicles--;
        return v;
    }
    else
    {
        throw runtime_error("Lane " + laneName + " is empty. Cannot remove vehicle.");
    }
}

int Lane::getVehicleCount() const
{
    return vehicleQueue.size();
}

bool Lane::hasEmergencyVehicle() const
{
    queue<Vehicle> tempQueue = vehicleQueue;
    while (!tempQueue.empty())
    {
        if (tempQueue.front().isEmergency())
            return true;
        tempQueue.pop();
    }
    return false;
}

void Lane::updateAllWaitingTimes(int currentTime)
{
    totalWaitingTime += getVehicleCount();
}

int Lane::getTotalWaitingTime() const
{
    return totalWaitingTime;
}

int Lane::getAverageWaitingTime() const
{
    if (totalVehicles == 0)
        return 0;
    return totalWaitingTime / totalVehicles;
}

double Lane::calculatePriority() const
{
    int vehicleCount = getVehicleCount();
    int avgWaitingTime = getAverageWaitingTime();
    int effectiveCapacity = getEffectiveCapacity();

    if(effectiveCapacity == 0)
        return 0.0;
    
    if(hasEmergencyVehicle())
        return 999999.0;

    return (double)(vehicleCount * avgWaitingTime) / effectiveCapacity;
}

void Lane::setBlockage(double percent)
{
    if (percent < 0.0) percent = 0.0;
    if (percent > 1.0) percent = 1.0;
    blockagePercent = percent;
    blocked = (percent > 0.0);
}

bool Lane::isBlocked() const
{
    return blocked;
}

int Lane::getEffectiveCapacity() const
{
    int eff = static_cast<int>(capacity * (1.0 - blockagePercent));
    return std::max(0, eff);
}

string Lane::getLaneName() const
{
    return laneName;
}

void Lane::displayLaneStatus() const
{
    cout << "Lane " << laneName << " status: " << (blocked ? "Blocked" : "Open")
         << ", vehicles=" << getVehicleCount()
         << ", effective capacity=" << getEffectiveCapacity()
         << ", avg waiting=" << getAverageWaitingTime() << endl;
}

bool Lane::isEmpty() const
{
    return vehicleQueue.empty();
}

bool Lane::isFull() const
{
    return getVehicleCount() >= getEffectiveCapacity();
}

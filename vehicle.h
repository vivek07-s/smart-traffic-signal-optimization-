#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>
using namespace std;

enum VehicleType
{
    REGULAR,
    EMERGENCY,
    PUBLIC_TRANSPORT
};

class Vehicle
{
    private:
        int vehicleID;
        time_t arrivalTime;
        int waitingTime;
        VehicleType type;


    public:
        Vehicle(int id, VehicleType vType = REGULAR);
        Vehicle(int id, time_t arrival, VehicleType vType = REGULAR);

        int getVehicleID() const;
        time_t getArrivalTime() const;
        int getWaitingTime() const;
        VehicleType getType() const;


        bool isEmergency() const;
        bool isPublicTransport() const;
        int getPriorityMultiplier() const;

        void updateWaitingTime(int currentTime);

        void displayVehicle() const;
        string getTypeString() const;

};

# endif

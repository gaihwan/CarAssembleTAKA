#pragma once
#include "car_config.h"
#include "IValidator.h"

class CarAssembler
{
public:
    explicit CarAssembler(IValidator* validator);
    void selectCarType(int answer);
    void selectEngine(int answer);
    void selectBrakeSystem(int answer);
    void selectSteeringSystem(int answer);
    void runCar();
    void runTest();
    void reset();
    const CarConfig& getConfig() const;
private:
    CarConfig   config_;
    IValidator* validator_;
};

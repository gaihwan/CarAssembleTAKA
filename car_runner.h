#pragma once
#include "car_config.h"

void selectCarType(CarConfig& config, int answer);
void selectEngine(CarConfig& config, int answer);
void selectBrakeSystem(CarConfig& config, int answer);
void selectSteeringSystem(CarConfig& config, int answer);
void runCar(const CarConfig& config);
void runTest(const CarConfig& config);

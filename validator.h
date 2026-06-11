#pragma once
#include "car_config.h"

bool isSedanContinentalConflict(const CarConfig& c);
bool isSuvToyotaConflict(const CarConfig& c);
bool isTruckWiaConflict(const CarConfig& c);
bool isTruckMandoConflict(const CarConfig& c);
bool isBoschBrakeWithoutBoschSteer(const CarConfig& c);
bool isValidCheck(const CarConfig& c);

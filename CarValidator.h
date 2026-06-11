#pragma once
#include "IValidator.h"

class CarValidator : public IValidator
{
public:
    bool isValid(const CarConfig& config) override;
private:
    bool checkSedanContinental(const CarConfig& c);
    bool checkSuvToyota(const CarConfig& c);
    bool checkTruckWia(const CarConfig& c);
    bool checkTruckMando(const CarConfig& c);
    bool checkBoschPair(const CarConfig& c);
};

#pragma once
#include "car_config.h"

class IValidator
{
public:
    virtual ~IValidator() = default;
    virtual bool isValid(const CarConfig& config) = 0;
};

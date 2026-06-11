## IValidator.h

```cpp
#pragma once
#include "car_config.h"

class IValidator
{
public:
    virtual ~IValidator() = default;
    virtual bool isValid(const CarConfig& config) = 0;
};
```

---

## CarValidator.h

```cpp
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
```

---

## CarValidator.cpp

```cpp
#include "CarValidator.h"

bool CarValidator::checkSedanContinental(const CarConfig& c)
{
    return c.carType == SEDAN && c.brake == CONTINENTAL;
}

bool CarValidator::checkSuvToyota(const CarConfig& c)
{
    return c.carType == SUV && c.engine == TOYOTA;
}

bool CarValidator::checkTruckWia(const CarConfig& c)
{
    return c.carType == TRUCK && c.engine == WIA;
}

bool CarValidator::checkTruckMando(const CarConfig& c)
{
    return c.carType == TRUCK && c.brake == MANDO;
}

bool CarValidator::checkBoschPair(const CarConfig& c)
{
    return c.brake == BOSCH_B && c.steering != BOSCH_S;
}

bool CarValidator::isValid(const CarConfig& config)
{
    if (checkSedanContinental(config)) return false;
    if (checkSuvToyota(config))        return false;
    if (checkTruckWia(config))         return false;
    if (checkTruckMando(config))       return false;
    if (checkBoschPair(config))        return false;
    return true;
}
```

## validator.h

```cpp
#pragma once
#include "car_config.h"

bool isSedanContinentalConflict(const CarConfig& c);
bool isSuvToyotaConflict(const CarConfig& c);
bool isTruckWiaConflict(const CarConfig& c);
bool isTruckMandoConflict(const CarConfig& c);
bool isBoschBrakeWithoutBoschSteer(const CarConfig& c);
bool isValidCheck(const CarConfig& c);
```

---

## validator.cpp

```cpp
#include "validator.h"

bool isSedanContinentalConflict(const CarConfig& c)
{
    return c.carType == SEDAN && c.brake == CONTINENTAL;
}

bool isSuvToyotaConflict(const CarConfig& c)
{
    return c.carType == SUV && c.engine == TOYOTA;
}

bool isTruckWiaConflict(const CarConfig& c)
{
    return c.carType == TRUCK && c.engine == WIA;
}

bool isTruckMandoConflict(const CarConfig& c)
{
    return c.carType == TRUCK && c.brake == MANDO;
}

bool isBoschBrakeWithoutBoschSteer(const CarConfig& c)
{
    return c.brake == BOSCH_B && c.steering != BOSCH_S;
}

bool isValidCheck(const CarConfig& c)
{
    if (isSedanContinentalConflict(c))    return false;
    if (isSuvToyotaConflict(c))           return false;
    if (isTruckWiaConflict(c))            return false;
    if (isTruckMandoConflict(c))          return false;
    if (isBoschBrakeWithoutBoschSteer(c)) return false;
    return true;
}
```

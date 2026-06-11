## car_runner.h

```cpp
#pragma once
#include "car_config.h"

void selectCarType(CarConfig& config, int answer);
void selectEngine(CarConfig& config, int answer);
void selectBrakeSystem(CarConfig& config, int answer);
void selectSteeringSystem(CarConfig& config, int answer);
void runCar(const CarConfig& config);
void runTest(const CarConfig& config);
```

---

## car_runner.cpp

```cpp
#include "car_runner.h"
#include "validator.h"
#include <stdio.h>

void selectCarType(CarConfig& config, int answer)
{
    config.carType = static_cast<CarType>(answer);
    if (config.carType == SEDAN) printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_SEDAN);
    if (config.carType == SUV)   printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_SUV);
    if (config.carType == TRUCK) printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_TRUCK);
}

void selectEngine(CarConfig& config, int answer)
{
    config.engine = static_cast<Engine>(answer);
    if (config.engine == GM)     printf("%s 엔진을 선택하셨습니다.\n", ENG_GM);
    if (config.engine == TOYOTA) printf("%s 엔진을 선택하셨습니다.\n", ENG_TOYOTA);
    if (config.engine == WIA)    printf("%s 엔진을 선택하셨습니다.\n", ENG_WIA);
}

void selectBrakeSystem(CarConfig& config, int answer)
{
    config.brake = static_cast<brakeSystem>(answer);
    if (config.brake == MANDO)       printf("%s 제동장치를 선택하셨습니다.\n", BRK_MANDO);
    if (config.brake == CONTINENTAL) printf("%s 제동장치를 선택하셨습니다.\n", BRK_CONTINENTAL);
    if (config.brake == BOSCH_B)     printf("%s 제동장치를 선택하셨습니다.\n", BRK_BOSCH);
}

void selectSteeringSystem(CarConfig& config, int answer)
{
    config.steering = static_cast<SteeringSystem>(answer);
    if (config.steering == BOSCH_S) printf("%s 조향장치를 선택하셨습니다.\n", STR_BOSCH);
    if (config.steering == MOBIS)   printf("%s 조향장치를 선택하셨습니다.\n", STR_MOBIS);
}

void runCar(const CarConfig& config)
{
    if (!isValidCheck(config))
    {
        printf("자동차가 동작되지 않습니다\n");
        return;
    }
    if (config.engine == BROKEN)
    {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
        return;
    }
    if (config.carType == SEDAN) printf("Car Type : %s\n", CAR_SEDAN);
    if (config.carType == SUV)   printf("Car Type : %s\n", CAR_SUV);
    if (config.carType == TRUCK) printf("Car Type : %s\n", CAR_TRUCK);
    if (config.engine == GM)     printf("Engine : %s\n", ENG_GM);
    if (config.engine == TOYOTA) printf("Engine : %s\n", ENG_TOYOTA);
    if (config.engine == WIA)    printf("Engine : %s\n", ENG_WIA);
    if (config.brake == MANDO)       printf("Brake System : %s\n", BRK_MANDO);
    if (config.brake == CONTINENTAL) printf("Brake System : %s\n", BRK_CONTINENTAL);
    if (config.brake == BOSCH_B)     printf("Brake System : %s\n", BRK_BOSCH);
    if (config.steering == BOSCH_S) printf("SteeringSystem : %s\n", STR_BOSCH);
    if (config.steering == MOBIS)   printf("SteeringSystem : %s\n", STR_MOBIS);
    printf("자동차가 동작됩니다.\n");
}

void runTest(const CarConfig& config)
{
    if (isSedanContinentalConflict(config))
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Sedan에는 Continental제동장치 사용 불가\n");
    }
    else if (isSuvToyotaConflict(config))
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("SUV에는 TOYOTA엔진 사용 불가\n");
    }
    else if (isTruckWiaConflict(config))
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 WIA엔진 사용 불가\n");
    }
    else if (isTruckMandoConflict(config))
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 Mando제동장치 사용 불가\n");
    }
    else if (isBoschBrakeWithoutBoschSteer(config))
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Bosch제동장치에는 Bosch조향장치 이외 사용 불가\n");
    }
    else
    {
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
    }
}
```

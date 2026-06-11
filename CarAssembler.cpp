#include "CarAssembler.h"
#include <stdio.h>

CarAssembler::CarAssembler(IValidator* validator)
    : validator_(validator)
{
}

void CarAssembler::selectCarType(int answer)
{
    config_.carType = static_cast<CarType>(answer);
    if (config_.carType == SEDAN) printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_SEDAN);
    if (config_.carType == SUV)   printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_SUV);
    if (config_.carType == TRUCK) printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_TRUCK);
}

void CarAssembler::selectEngine(int answer)
{
    config_.engine = static_cast<Engine>(answer);
    if (config_.engine == GM)     printf("%s 엔진을 선택하셨습니다.\n", ENG_GM);
    if (config_.engine == TOYOTA) printf("%s 엔진을 선택하셨습니다.\n", ENG_TOYOTA);
    if (config_.engine == WIA)    printf("%s 엔진을 선택하셨습니다.\n", ENG_WIA);
}

void CarAssembler::selectBrakeSystem(int answer)
{
    config_.brake = static_cast<brakeSystem>(answer);
    if (config_.brake == MANDO)       printf("%s 제동장치를 선택하셨습니다.\n", BRK_MANDO);
    if (config_.brake == CONTINENTAL) printf("%s 제동장치를 선택하셨습니다.\n", BRK_CONTINENTAL);
    if (config_.brake == BOSCH_B)     printf("%s 제동장치를 선택하셨습니다.\n", BRK_BOSCH);
}

void CarAssembler::selectSteeringSystem(int answer)
{
    config_.steering = static_cast<SteeringSystem>(answer);
    if (config_.steering == BOSCH_S) printf("%s 조향장치를 선택하셨습니다.\n", STR_BOSCH);
    if (config_.steering == MOBIS)   printf("%s 조향장치를 선택하셨습니다.\n", STR_MOBIS);
}

void CarAssembler::runCar()
{
    if (!validator_->isValid(config_))
    {
        printf("자동차가 동작되지 않습니다\n");
        return;
    }
    if (config_.engine == BROKEN)
    {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
        return;
    }
    if (config_.carType == SEDAN) printf("Car Type : %s\n", CAR_SEDAN);
    if (config_.carType == SUV)   printf("Car Type : %s\n", CAR_SUV);
    if (config_.carType == TRUCK) printf("Car Type : %s\n", CAR_TRUCK);
    if (config_.engine == GM)     printf("Engine : %s\n", ENG_GM);
    if (config_.engine == TOYOTA) printf("Engine : %s\n", ENG_TOYOTA);
    if (config_.engine == WIA)    printf("Engine : %s\n", ENG_WIA);
    if (config_.brake == MANDO)       printf("Brake System : %s\n", BRK_MANDO);
    if (config_.brake == CONTINENTAL) printf("Brake System : %s\n", BRK_CONTINENTAL);
    if (config_.brake == BOSCH_B)     printf("Brake System : %s\n", BRK_BOSCH);
    if (config_.steering == BOSCH_S) printf("SteeringSystem : %s\n", STR_BOSCH);
    if (config_.steering == MOBIS)   printf("SteeringSystem : %s\n", STR_MOBIS);
    printf("자동차가 동작됩니다.\n");
}

void CarAssembler::runTest()
{
    if (validator_->isValid(config_))
        printf("자동차 부품 조합 테스트 결과 : PASS\n");
    else
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
}

void CarAssembler::reset()
{
    config_ = CarConfig{};
}

const CarConfig& CarAssembler::getConfig() const
{
    return config_;
}

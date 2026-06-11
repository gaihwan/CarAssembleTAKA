## CarAssembler.h

```cpp
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
```

---

## CarAssembler.cpp

```cpp
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
```

---

## main.cpp (Phase 3)

```cpp
// CarAssemble - main.cpp
// Phase 3: CarAssembler + CarValidator 사용. main()은 UI 루프만 담당.

#ifdef _DEBUG
#include "gmock/gmock.h"
int main() { testing::InitGoogleMock(); return RUN_ALL_TESTS(); }
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "car_config.h"
#include "CarValidator.h"
#include "CarAssembler.h"

#define CLEAR_SCREEN "\033[H\033[2J"

void delay(int ms) { /* busy-wait loop */ }

int main()
{
    char buf[100];
    int step = CarType_Q;
    CarValidator validator;
    CarAssembler assembler(&validator);  // ← Phase 3: DI(의존성 주입)

    while (1)
    {
        // ... 각 step 별 메뉴 출력 ...
        if (step == CarType_Q) { assembler.selectCarType(answer); step = Engine_Q; }
        else if (step == Engine_Q) { assembler.selectEngine(answer); step = brakeSystem_Q; }
        else if (step == brakeSystem_Q) { assembler.selectBrakeSystem(answer); step = SteeringSystem_Q; }
        else if (step == SteeringSystem_Q) { assembler.selectSteeringSystem(answer); step = Run_Test; }
        else if (step == Run_Test && answer == 1) { assembler.runCar(); }
        else if (step == Run_Test && answer == 2) { assembler.runTest(); }
        // answer==0 && step==Run_Test: assembler.reset(); step = CarType_Q;
    }
    return 0;
}
#endif
```

---

## test/test_phase3.cpp

```cpp
// Phase 3 Unit Tests — test_phase3.cpp
// PLANS.md Phase 3-2, 3-3 검증: CarValidator + MockValidator로 CarAssembler 독립 테스트

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../car_config.h"
#include "../IValidator.h"
#include "../CarValidator.h"
#include "../CarAssembler.h"

using ::testing::Return;
using ::testing::_;

// -----------------------------------------------
// [P3-T1] CarValidator 클래스 규칙별 테스트 (Phase 3-2)
// -----------------------------------------------
class CarValidatorTest : public ::testing::Test
{
protected:
    CarValidator validator;
};

TEST_F(CarValidatorTest, SedanContinental_IsInvalid)
{
    CarConfig c{ SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, SuvToyota_IsInvalid)
{
    CarConfig c{ SUV, TOYOTA, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, TruckWia_IsInvalid)
{
    CarConfig c{ TRUCK, WIA, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, TruckMando_IsInvalid)
{
    CarConfig c{ TRUCK, GM, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, BoschBrakeWithoutBoschSteer_IsInvalid)
{
    CarConfig c{ SEDAN, GM, BOSCH_B, MOBIS };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, ValidConfig_ReturnsTrue)
{
    CarConfig c{ TRUCK, GM, CONTINENTAL, BOSCH_S };
    EXPECT_TRUE(validator.isValid(c));
}

// -----------------------------------------------
// MockValidator 정의
// -----------------------------------------------
class MockValidator : public IValidator
{
public:
    MOCK_METHOD(bool, isValid, (const CarConfig&), (override));
};

// -----------------------------------------------
// [P3-T2] MockValidator로 CarAssembler 독립 테스트 (Phase 3-3)
// runCar(): 유효 시 "Car Type" 출력, 무효/고장 시 미출력 — ASCII 비교
// runTest(): 유효 시 "PASS", 무효 시 "FAIL" 출력 — ASCII 비교
// -----------------------------------------------

TEST(CarAssemblerTest, RunCar_ValidConfig_PrintsCarType)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));
    CarAssembler assembler(&mock);
    assembler.selectCarType(1);        // SEDAN
    assembler.selectEngine(1);         // GM
    assembler.selectBrakeSystem(1);    // MANDO
    assembler.selectSteeringSystem(1); // BOSCH_S
    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Car Type"), std::string::npos);
}

TEST(CarAssemblerTest, RunCar_InvalidConfig_DoesNotPrintCarType)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));
    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("Car Type"), std::string::npos);
}

TEST(CarAssemblerTest, RunCar_BrokenEngine_DoesNotPrintCarType)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));
    CarAssembler assembler(&mock);
    assembler.selectEngine(4);  // BROKEN
    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("Car Type"), std::string::npos);
}

TEST(CarAssemblerTest, RunTest_ValidConfig_PrintsPass)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));
    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runTest();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("PASS"), std::string::npos);
}

TEST(CarAssemblerTest, RunTest_InvalidConfig_PrintsFail)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));
    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runTest();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("FAIL"), std::string::npos);
}

TEST(CarAssemblerTest, SelectCarType_StoresCorrectly)
{
    MockValidator mock;
    CarAssembler assembler(&mock);
    assembler.selectCarType(2);  // SUV
    EXPECT_EQ(assembler.getConfig().carType, SUV);
}

TEST(CarAssemblerTest, SelectEngine_StoresCorrectly)
{
    MockValidator mock;
    CarAssembler assembler(&mock);
    assembler.selectEngine(3);  // WIA
    EXPECT_EQ(assembler.getConfig().engine, WIA);
}
```

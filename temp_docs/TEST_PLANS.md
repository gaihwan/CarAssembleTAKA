# CarAssemble Unit Test Plan (GMock)

> 참조: `temp_docs/PLANS.md`  
> 테스트 프레임워크: GMock 1.11.0 (NuGet)  
> 원칙: **각 Phase 리팩토링 완료 후 해당 Phase 테스트 통과 확인 → commit**

---

## 테스트 파일 구조

```
CarAssemble/
├── test/
│   ├── test_phase1.cpp   ← Phase 1 함수 단위 테스트
│   ├── test_phase2.cpp   ← Phase 2 모듈/구조체 테스트
│   └── test_phase3.cpp   ← Phase 3 클래스 + GMock 테스트
└── ...
```

---

## Phase 1 테스트 — Method Level

> PLANS.md Phase 1-1 ~ 1-3 완료 후 실행

### 대상 파일: `test/test_phase1.cpp`

---

### [P1-T1] BROKEN 엔진 enum 등록 확인 (Phase 1-2)

```cpp
TEST(EnumTest, BrokenEngine_IsDefinedAsValue4) {
    EXPECT_EQ(static_cast<int>(BROKEN), 4);
}
```

**목적:** 매직 넘버 `4` 대신 `BROKEN` enum이 올바른 값으로 정의됐는지 확인

---

### [P1-T2] 각 규칙 함수 — 충돌 감지 (Phase 1-3)

각 함수에 전역 `stack[]` 값을 세팅한 후 반환값 검증.

```cpp
// 규칙 1: Sedan + Continental = conflict
TEST(RuleTest, SedanContinental_DetectsConflict) {
    stack[CarType_Q]    = SEDAN;
    stack[brakeSystem_Q] = CONTINENTAL;
    EXPECT_TRUE(isSedanContinentalConflict());
}

TEST(RuleTest, SedanMando_NoConflict) {
    stack[CarType_Q]    = SEDAN;
    stack[brakeSystem_Q] = MANDO;
    EXPECT_FALSE(isSedanContinentalConflict());
}

// 규칙 2: SUV + Toyota = conflict
TEST(RuleTest, SuvToyota_DetectsConflict) {
    stack[CarType_Q] = SUV;
    stack[Engine_Q]  = TOYOTA;
    EXPECT_TRUE(isSuvToyotaConflict());
}

TEST(RuleTest, SuvGM_NoConflict) {
    stack[CarType_Q] = SUV;
    stack[Engine_Q]  = GM;
    EXPECT_FALSE(isSuvToyotaConflict());
}

// 규칙 3: Truck + WIA = conflict
TEST(RuleTest, TruckWia_DetectsConflict) {
    stack[CarType_Q] = TRUCK;
    stack[Engine_Q]  = WIA;
    EXPECT_TRUE(isTruckWiaConflict());
}

// 규칙 4: Truck + Mando = conflict
TEST(RuleTest, TruckMando_DetectsConflict) {
    stack[CarType_Q]     = TRUCK;
    stack[brakeSystem_Q] = MANDO;
    EXPECT_TRUE(isTruckMandoConflict());
}

// 규칙 5: BoschBrake → BoschSteering 필수
TEST(RuleTest, BoschBrakeWithoutBoschSteer_DetectsConflict) {
    stack[brakeSystem_Q]    = BOSCH_B;
    stack[SteeringSystem_Q] = MOBIS;
    EXPECT_TRUE(isBoschBrakeWithoutBoschSteer());
}

TEST(RuleTest, BoschBrakeWithBoschSteer_NoConflict) {
    stack[brakeSystem_Q]    = BOSCH_B;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_FALSE(isBoschBrakeWithoutBoschSteer());
}
```

---

### [P1-T3] isValidCheck() 종합 검증 (Phase 1-3)

```cpp
// 모든 규칙 위반 없음 → valid
TEST(ValidCheckTest, AllValid_ReturnsTrue) {
    stack[CarType_Q]        = SEDAN;
    stack[Engine_Q]         = GM;
    stack[brakeSystem_Q]    = MANDO;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_TRUE(isValidCheck());
}

// 규칙 1 위반
TEST(ValidCheckTest, SedanContinental_ReturnsFalse) {
    stack[CarType_Q]        = SEDAN;
    stack[Engine_Q]         = GM;
    stack[brakeSystem_Q]    = CONTINENTAL;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_FALSE(isValidCheck());
}

// 규칙 5 위반
TEST(ValidCheckTest, BoschBrakeWithMobis_ReturnsFalse) {
    stack[CarType_Q]        = SUV;
    stack[Engine_Q]         = GM;
    stack[brakeSystem_Q]    = BOSCH_B;
    stack[SteeringSystem_Q] = MOBIS;
    EXPECT_FALSE(isValidCheck());
}
```

---

## Phase 2 테스트 — Module Level

> PLANS.md Phase 2-1 ~ 2-2 완료 후 실행

### 대상 파일: `test/test_phase2.cpp`

---

### [P2-T1] CarConfig 구조체 기본 초기화 (Phase 2-2)

```cpp
TEST(CarConfigTest, DefaultInitialization) {
    CarConfig config{};
    EXPECT_EQ(config.carType,  static_cast<CarType>(0));
    EXPECT_EQ(config.engine,   static_cast<Engine>(0));
    EXPECT_EQ(config.brake,    static_cast<brakeSystem>(0));
    EXPECT_EQ(config.steering, static_cast<SteeringSystem>(0));
}
```

---

### [P2-T2] isValidCheck(CarConfig) 오버로드 — 규칙별 검증 (Phase 2-2)

전역 `stack[]` 대신 `CarConfig`를 인자로 받는 버전 테스트.

```cpp
TEST(CarConfigValidationTest, Rule1_SedanContinental_ReturnsFalse) {
    CarConfig c{ SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule2_SuvToyota_ReturnsFalse) {
    CarConfig c{ SUV, TOYOTA, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule3_TruckWia_ReturnsFalse) {
    CarConfig c{ TRUCK, WIA, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule4_TruckMando_ReturnsFalse) {
    CarConfig c{ TRUCK, GM, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule5_BoschBrakeWithoutBoschSteer_ReturnsFalse) {
    CarConfig c{ SEDAN, GM, BOSCH_B, MOBIS };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, AllRulesPass_ReturnsTrue) {
    CarConfig c{ SEDAN, GM, MANDO, BOSCH_S };
    EXPECT_TRUE(isValidCheck(c));
}
```

---

## Phase 3 테스트 — Class Level (GMock)

> PLANS.md Phase 3-1 ~ 3-3 완료 후 실행

### 대상 파일: `test/test_phase3.cpp`

---

### [P3-T1] CarValidator — 각 규칙 메서드 단위 테스트 (Phase 3-2)

```cpp
#include "CarValidator.h"

class CarValidatorTest : public ::testing::Test {
protected:
    CarValidator validator;
};

// 규칙 1
TEST_F(CarValidatorTest, SedanContinental_IsInvalid) {
    CarConfig c{ SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

// 규칙 2
TEST_F(CarValidatorTest, SuvToyota_IsInvalid) {
    CarConfig c{ SUV, TOYOTA, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

// 규칙 3
TEST_F(CarValidatorTest, TruckWia_IsInvalid) {
    CarConfig c{ TRUCK, WIA, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

// 규칙 4
TEST_F(CarValidatorTest, TruckMando_IsInvalid) {
    CarConfig c{ TRUCK, GM, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

// 규칙 5
TEST_F(CarValidatorTest, BoschBrakeWithoutBoschSteer_IsInvalid) {
    CarConfig c{ SEDAN, GM, BOSCH_B, MOBIS };
    EXPECT_FALSE(validator.isValid(c));
}

// 정상 조합
TEST_F(CarValidatorTest, ValidConfig_ReturnsTrue) {
    CarConfig c{ TRUCK, GM, CONTINENTAL, BOSCH_S };
    EXPECT_TRUE(validator.isValid(c));
}
```

---

### [P3-T2] MockValidator로 CarAssembler 독립 테스트 (Phase 3-3)

```cpp
#include "CarAssembler.h"
#include "IValidator.h"
#include <gmock/gmock.h>

using ::testing::Return;
using ::testing::_;

class MockValidator : public IValidator {
public:
    MOCK_METHOD(bool, isValid, (const CarConfig&), (override));
};
```

#### runCar() 테스트

```cpp
TEST(CarAssemblerTest, RunCar_ValidConfig_PrintsRunning) {
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));

    CarAssembler assembler(&mock);
    assembler.selectCarType(1);      // SEDAN
    assembler.selectEngine(1);       // GM
    assembler.selectBrakeSystem(1);  // MANDO
    assembler.selectSteeringSystem(1); // BOSCH_S

    // 출력 캡처: "자동차가 동작됩니다" 포함 여부 확인
    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("자동차가 동작됩니다"), std::string::npos);
}

TEST(CarAssemblerTest, RunCar_InvalidConfig_PrintsError) {
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));

    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("자동차가 동작되지 않습니다"), std::string::npos);
}

TEST(CarAssemblerTest, RunCar_BrokenEngine_PrintsBroken) {
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));

    CarAssembler assembler(&mock);
    assembler.selectEngine(4);  // BROKEN

    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("엔진이 고장나있습니다"), std::string::npos);
}
```

#### runTest() 테스트

```cpp
TEST(CarAssemblerTest, RunTest_ValidConfig_PrintsPass) {
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));

    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runTest();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("PASS"), std::string::npos);
}

TEST(CarAssemblerTest, RunTest_InvalidConfig_PrintsFail) {
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));

    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runTest();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("FAIL"), std::string::npos);
}
```

#### 부품 선택 저장 확인

```cpp
TEST(CarAssemblerTest, SelectCarType_StoresCorrectly) {
    MockValidator mock;
    CarAssembler assembler(&mock);
    assembler.selectCarType(2);  // SUV
    EXPECT_EQ(assembler.getConfig().carType, SUV);
}

TEST(CarAssemblerTest, SelectEngine_StoresCorrectly) {
    MockValidator mock;
    CarAssembler assembler(&mock);
    assembler.selectEngine(3);  // WIA
    EXPECT_EQ(assembler.getConfig().engine, WIA);
}
```

---

## Phase별 테스트 통과 기준 & Commit 조건

| Phase | 테스트 파일 | 통과 기준 | Commit 메시지 예시 |
|-------|------------|-----------|-------------------|
| Phase 1-2 | test_phase1.cpp | P1-T1 통과 | `refactor: add BROKEN enum, remove magic number` |
| Phase 1-3 | test_phase1.cpp | P1-T2, P1-T3 통과 | `refactor: extract individual rule functions` |
| Phase 2-2 | test_phase2.cpp | P2-T1, P2-T2 통과 | `refactor: replace stack[] with CarConfig struct` |
| Phase 3-2 | test_phase3.cpp | P3-T1 통과 | `refactor: implement CarValidator class` |
| Phase 3-3 | test_phase3.cpp | P3-T2 통과 | `refactor: implement CarAssembler with IValidator` |

---

## GMock 테스트 프로젝트 설정 (Visual Studio)

1. 솔루션에 새 프로젝트 추가: **빈 프로젝트 (C++)** → 이름: `CarAssembleTest`
2. NuGet 패키지 설치: `gmock 1.11.0`
3. 프로젝트 참조: `CarAssemble` 프로젝트의 헤더 경로 추가
4. `test/` 폴더의 `.cpp` 파일을 프로젝트에 포함
5. `main` 진입점:

```cpp
// test_main.cpp
#include <gtest/gtest.h>
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

# CarAssemble 세션 작업 기록 — Phase 2 (Module Level 리팩토링)

---

## 9. Phase 2 리팩토링 실행

**사용자 요청:**
> 좋아. Phase 2도 동일하게 진행해주고, unittest 도 진행해주고, history 문서까지 남겨줘.

**작업 흐름:** Phase 2-1 파일 분리 → Phase 2-2 CarConfig 구조체 도입 → test_phase1.cpp 수정 → test_phase2.cpp 작성 → 빌드 → 28/28 PASS → commit → lecture_docs 저장

---

### Phase 2-1: 파일 분리

`main.cpp` 단일 파일에서 책임별로 분리:

| 파일 | 역할 |
|------|------|
| `car_config.h` | 모든 enum, 문자열 상수, CarConfig 구조체 정의 |
| `validator.h` / `validator.cpp` | 5개 규칙 함수 + isValidCheck() |
| `car_runner.h` / `car_runner.cpp` | selectXxx(), runCar(), runTest() |
| `main.cpp` | `#ifdef _DEBUG` → GMock 러너 / `#else` → UI 루프만 |

`CarAssemble.vcxproj` 업데이트:
```xml
<!-- 항상 컴파일 -->
<ClCompile Include="car_runner.cpp" />
<ClCompile Include="validator.cpp" />
<!-- Debug 전용 -->
<ClCompile Include="test\test_phase2.cpp">
  <ExcludedFromBuild Condition="'$(Configuration)'=='Release'">true</ExcludedFromBuild>
</ClCompile>
<!-- 헤더 등록 -->
<ClInclude Include="car_config.h" />
<ClInclude Include="car_runner.h" />
<ClInclude Include="validator.h" />
```

---

### Phase 2-2: 전역 stack[] → CarConfig 구조체

**before:**
```cpp
int stack[10];   // 전역 배열로 상태 관리
// stack[CarType_Q], stack[Engine_Q] ... 로 접근
```

**after (car_config.h):**
```cpp
struct CarConfig {
    CarType        carType  = static_cast<CarType>(0);
    Engine         engine   = static_cast<Engine>(0);
    brakeSystem    brake    = static_cast<brakeSystem>(0);
    SteeringSystem steering = static_cast<SteeringSystem>(0);
};
```

함수 시그니처 변경:

| 파일 | Before | After |
|------|--------|-------|
| validator.cpp | `bool isSedanContinentalConflict()` (전역 stack 접근) | `bool isSedanContinentalConflict(const CarConfig& c)` |
| car_runner.cpp | `void selectCarType(int answer)` | `void selectCarType(CarConfig& config, int answer)` |
| main.cpp | `selectCarType(answer)` | `selectCarType(config, answer)` |

---

### 신규 파일 전체 내용

#### car_config.h

```cpp
#pragma once

// --- 문자열 상수 ---
constexpr const char* CAR_SEDAN       = "Sedan";
constexpr const char* CAR_SUV         = "SUV";
constexpr const char* CAR_TRUCK       = "Truck";
constexpr const char* ENG_GM          = "GM";
constexpr const char* ENG_TOYOTA      = "TOYOTA";
constexpr const char* ENG_WIA         = "WIA";
constexpr const char* BRK_MANDO       = "Mando";
constexpr const char* BRK_CONTINENTAL = "Continental";
constexpr const char* BRK_BOSCH       = "Bosch";
constexpr const char* STR_BOSCH       = "Bosch";
constexpr const char* STR_MOBIS       = "Mobis";

// --- Enums ---
enum QuestionType { CarType_Q, Engine_Q, brakeSystem_Q, SteeringSystem_Q, Run_Test };
enum CarType      { SEDAN=1, SUV, TRUCK };
enum Engine       { GM=1, TOYOTA, WIA, BROKEN };
enum brakeSystem  { MANDO=1, CONTINENTAL, BOSCH_B };
enum SteeringSystem { BOSCH_S=1, MOBIS };

// --- Phase 2-2: 전역 stack[] 대체 구조체 ---
struct CarConfig {
    CarType        carType  = static_cast<CarType>(0);
    Engine         engine   = static_cast<Engine>(0);
    brakeSystem    brake    = static_cast<brakeSystem>(0);
    SteeringSystem steering = static_cast<SteeringSystem>(0);
};
```

#### validator.h

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

#### validator.cpp

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

#### car_runner.h

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

#### car_runner.cpp (핵심 부분)

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

void runCar(const CarConfig& config)
{
    if (!isValidCheck(config)) { printf("자동차가 동작되지 않습니다\n"); return; }
    if (config.engine == BROKEN) { printf("엔진이 고장나있습니다.\n"); return; }
    // ... 각 부품 출력 후
    printf("자동차가 동작됩니다.\n");
}

void runTest(const CarConfig& config)
{
    if      (isSedanContinentalConflict(config)) { printf("FAIL\nSedan에는 Continental제동장치 사용 불가\n"); }
    else if (isSuvToyotaConflict(config))        { printf("FAIL\nSUV에는 TOYOTA엔진 사용 불가\n"); }
    else if (isTruckWiaConflict(config))         { printf("FAIL\nTruck에는 WIA엔진 사용 불가\n"); }
    else if (isTruckMandoConflict(config))       { printf("FAIL\nTruck에는 Mando제동장치 사용 불가\n"); }
    else if (isBoschBrakeWithoutBoschSteer(config)) { printf("FAIL\nBosch제동장치에는 Bosch조향장치 이외 사용 불가\n"); }
    else                                         { printf("자동차 부품 조합 테스트 결과 : PASS\n"); }
}
```

#### main.cpp (Phase 2 — UI 루프만 담당)

```cpp
#ifdef _DEBUG
#include "gmock/gmock.h"
int main() { testing::InitGoogleMock(); return RUN_ALL_TESTS(); }
#else
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "car_config.h"
#include "car_runner.h"

#define CLEAR_SCREEN "\033[H\033[2J"

void delay(int ms) { /* busy-wait loop */ }

int main()
{
    char buf[100];
    int step = CarType_Q;
    CarConfig config;   // ← Phase 2: 전역 stack[] 대신 로컬 구조체

    while (1)
    {
        // ... 각 step 별 메뉴 출력 ...
        if (step == CarType_Q) { selectCarType(config, answer); step = Engine_Q; }
        else if (step == Engine_Q) { selectEngine(config, answer); step = brakeSystem_Q; }
        // ...
    }
    return 0;
}
#endif
```

---

### test_phase1.cpp 수정 (Phase 2 함수 시그니처 변경 대응)

Phase 1에서 `extern int stack[]` + 인수 없는 extern 함수 방식이었으나,
Phase 2에서 함수 시그니처가 `const CarConfig&`로 변경되어 재작성:

```cpp
// Before (Phase 1 방식)
extern int stack[];
extern bool isSedanContinentalConflict();  // 인수 없음

// After (Phase 2 방식)
#include "../car_config.h"
#include "../validator.h"

TEST(RuleTest, SedanContinental_DetectsConflict) {
    CarConfig c;
    c.carType = SEDAN;
    c.brake = CONTINENTAL;
    EXPECT_TRUE(isSedanContinentalConflict(c));
}
```

17개 테스트 모두 `CarConfig` API 방식으로 재작성 → 동일하게 17 PASS

---

### test/test_phase2.cpp 작성 (신규)

**[P2-T1] CarConfig 구조체 기본 초기화 — 3개 테스트:**

```cpp
TEST(CarConfigTest, DefaultInitialization)
{
    CarConfig config{};
    EXPECT_EQ(config.carType,  static_cast<CarType>(0));
    EXPECT_EQ(config.engine,   static_cast<Engine>(0));
    EXPECT_EQ(config.brake,    static_cast<brakeSystem>(0));
    EXPECT_EQ(config.steering, static_cast<SteeringSystem>(0));
}

TEST(CarConfigTest, FieldAssignment)
{
    CarConfig config;
    config.carType  = SEDAN; config.engine = GM;
    config.brake    = MANDO; config.steering = BOSCH_S;
    EXPECT_EQ(config.carType, SEDAN);
    // ...
}

TEST(CarConfigTest, AggregateInitialization)
{
    CarConfig c{ TRUCK, WIA, BOSCH_B, MOBIS };
    EXPECT_EQ(c.carType, TRUCK);
    // ...
}
```

**[P2-T2] isValidCheck(CarConfig) — 8개 테스트:**

```cpp
TEST(CarConfigValidationTest, Rule1_SedanContinental_ReturnsFalse)
{
    CarConfig c{ SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}
// Rule2 ~ Rule5 동일 패턴 ...

TEST(CarConfigValidationTest, AllRulesPass_ReturnsTrue)
{
    CarConfig c{ SEDAN, GM, MANDO, BOSCH_S };
    EXPECT_TRUE(isValidCheck(c));
}

TEST(CarConfigValidationTest, BoschBrakeWithBoschSteer_ReturnsTrue)
{
    CarConfig c{ SEDAN, GM, BOSCH_B, BOSCH_S };
    EXPECT_TRUE(isValidCheck(c));
}
```

---

### 빌드 및 테스트 실행 결과

```
MSBuild CarAssemble.sln /p:Configuration=Debug /p:Platform=x64
빌드 결과: 성공

[==========] Running 28 tests from 5 test suites.
[----------] 1 test from EnumTest           → 1 PASSED
[----------] 10 tests from RuleTest         → 10 PASSED
[----------] 6 tests from ValidCheckTest    → 6 PASSED
[----------] 3 tests from CarConfigTest     → 3 PASSED
[----------] 8 tests from CarConfigValidationTest → 8 PASSED
[==========] 28 tests from 5 test suites ran.
[  PASSED  ] 28 tests.
```

---

### Phase 2 commit

```
refactor: Phase 2 - module level separation + CarConfig struct (all 28 tests PASS)

Phase 2-1: extract validator.h/cpp, car_runner.h/cpp from main.cpp
Phase 2-2: replace global stack[] with CarConfig struct
           - all validator/runner functions now take const CarConfig& or CarConfig&
           - main() declares local CarConfig config and passes it to functions

test: update test_phase1.cpp to use CarConfig API (extern stack[] approach removed)
test: add test_phase2.cpp with 11 new tests (P2-T1 struct init, P2-T2 isValidCheck)
```

커밋 해시: `e7df427`

---

### Phase 2 lecture_docs 파일 저장

| 파일 | 내용 |
|------|------|
| `lecture_docs/Phase2_car_config_h.md` | car_config.h 전체 소스 |
| `lecture_docs/Phase2_validator.md` | validator.h + validator.cpp 전체 소스 |
| `lecture_docs/Phase2_car_runner.md` | car_runner.h + car_runner.cpp 전체 소스 |
| `lecture_docs/Phase2_main_and_test.md` | main.cpp (Phase 2) + test_phase2.cpp 전체 소스 |

---

## Git 커밋 히스토리 (Phase 2 완료 시점)

| 커밋 해시 | 메시지 |
|-----------|--------|
| `fa42086` | Initial commit: CarAssemble C++ project |
| `8b51cf1` | docs: add CLAUDE.md, PLANS.md, TEST_PLANS.md for refactoring workflow |
| `206c98c` | refactor: Phase 1 - method level cleanup (all 17 tests PASS) |
| `fd26d2d` | docs: add Phase1 source snapshots to lecture_docs |
| `69bec13` | docs: add session work log to lecture_docs |
| `e7df427` | refactor: Phase 2 - module level separation + CarConfig struct (all 28 tests PASS) |
| `d6d3586` | docs: add Phase 2 source snapshots and update session work log |

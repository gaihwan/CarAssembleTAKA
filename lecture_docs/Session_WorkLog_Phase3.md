# CarAssemble 세션 작업 기록 — Phase 3 (Class Level 리팩토링)

---

## 10. Phase 3 리팩토링 실행

**사용자 요청:**
> Phase 3도 동일하게 진행해줘

**작업 흐름:** Phase 3-1 IValidator → Phase 3-2 CarValidator → Phase 3-3 CarAssembler → test_phase3.cpp 작성 → 빌드 → 41/41 PASS → commit → lecture_docs 저장

---

### Phase 3-1: IValidator 인터페이스 추출

OCP 적용의 핵심 — 검증 로직을 추상화하여 CarAssembler가 구체 구현에 의존하지 않도록 한다.

```cpp
// IValidator.h
#pragma once
#include "car_config.h"

class IValidator
{
public:
    virtual ~IValidator() = default;
    virtual bool isValid(const CarConfig& config) = 0;
};
```

**신규 파일:** `IValidator.h`

---

### Phase 3-2: CarValidator 클래스 구현

IValidator를 상속받아 5개 규칙을 private 메서드로 캡슐화한 구체 클래스.

```cpp
// CarValidator.h
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

```cpp
// CarValidator.cpp
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

Phase 2의 자유 함수(`validator.cpp`)와 별개로 독립 구현 — 기존 자유 함수 테스트(Phase 1/2)는 그대로 유지됨.

**신규 파일:** `CarValidator.h`, `CarValidator.cpp`

---

### Phase 3-3: CarAssembler 클래스 구현

부품 선택 상태(`CarConfig`)와 검증 의존성(`IValidator*`)을 함께 캡슐화.

```cpp
// CarAssembler.h
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

**OCP 적용 포인트:**
- `runTest()`는 `validator_->isValid(config_)`만 호출 → 어떤 규칙이 추가/변경되더라도 CarAssembler 수정 불필요
- `runCar()`도 동일하게 isValid() 경유
- 새로운 검증 정책이 필요하면 IValidator를 구현한 새 클래스만 추가

**신규 파일:** `CarAssembler.h`, `CarAssembler.cpp`

---

### main.cpp 업데이트

Phase 2의 자유 함수 직접 호출 방식 → Phase 3의 CarAssembler 위임 방식으로 전환.

```cpp
// Phase 2 (before)
CarConfig config;
selectCarType(config, answer);
runCar(config);
config = CarConfig{};

// Phase 3 (after)
CarValidator validator;
CarAssembler assembler(&validator);  // 의존성 주입(DI)
assembler.selectCarType(answer);
assembler.runCar();
assembler.reset();
```

`#include "car_runner.h"` 제거 → `#include "CarValidator.h"`, `#include "CarAssembler.h"` 로 교체.

---

### CarAssemble.vcxproj 업데이트

| 항목 | 내용 |
|------|------|
| 신규 ClCompile | `CarAssembler.cpp`, `CarValidator.cpp` (항상 컴파일) |
| 신규 ClCompile (Debug only) | `test\test_phase3.cpp` |
| 신규 ClInclude | `CarAssembler.h`, `CarValidator.h`, `IValidator.h` |

---

### test/test_phase3.cpp 작성

**[P3-T1] CarValidator 규칙별 테스트 — 6개:**

`CarValidatorTest` fixture 사용 (`::testing::Test` 상속).

| 테스트 | 입력 | 기대값 |
|--------|------|--------|
| SedanContinental_IsInvalid | SEDAN+CONTINENTAL | false |
| SuvToyota_IsInvalid | SUV+TOYOTA | false |
| TruckWia_IsInvalid | TRUCK+WIA | false |
| TruckMando_IsInvalid | TRUCK+MANDO | false |
| BoschBrakeWithoutBoschSteer_IsInvalid | BOSCH_B+MOBIS | false |
| ValidConfig_ReturnsTrue | TRUCK+GM+CONTINENTAL | true |

**[P3-T2] MockValidator로 CarAssembler 독립 테스트 — 7개:**

```cpp
class MockValidator : public IValidator {
public:
    MOCK_METHOD(bool, isValid, (const CarConfig&), (override));
};
```

stdout 캡처 시 한글 인코딩 리스크를 피하기 위해 ASCII 서브스트링으로 검증:

| 테스트 | Mock 동작 | 검증 방식 |
|--------|-----------|-----------|
| RunCar_ValidConfig_PrintsCarType | Return(true) | `find("Car Type") != npos` |
| RunCar_InvalidConfig_DoesNotPrintCarType | Return(false) | `find("Car Type") == npos` |
| RunCar_BrokenEngine_DoesNotPrintCarType | Return(true) + engine=BROKEN | `find("Car Type") == npos` |
| RunTest_ValidConfig_PrintsPass | Return(true) | `find("PASS") != npos` |
| RunTest_InvalidConfig_PrintsFail | Return(false) | `find("FAIL") != npos` |
| SelectCarType_StoresCorrectly | — | `getConfig().carType == SUV` |
| SelectEngine_StoresCorrectly | — | `getConfig().engine == WIA` |

---

### 빌드 및 테스트 실행 결과

```
MSBuild CarAssemble.sln /p:Configuration=Debug /p:Platform=x64
빌드 결과: 성공 (경고: C4819 코드 페이지 — 한글 포함, 무해)

[==========] Running 41 tests from 7 test suites.
[----------] 1 test from EnumTest           → 1 PASSED   (Phase 1)
[----------] 10 tests from RuleTest         → 10 PASSED  (Phase 1)
[----------] 6 tests from ValidCheckTest    → 6 PASSED   (Phase 1)
[----------] 3 tests from CarConfigTest     → 3 PASSED   (Phase 2)
[----------] 8 tests from CarConfigValidationTest → 8 PASSED (Phase 2)
[----------] 6 tests from CarValidatorTest  → 6 PASSED   (Phase 3)
[----------] 7 tests from CarAssemblerTest  → 7 PASSED   (Phase 3)
[==========] 41 tests from 7 test suites ran. (14 ms total)
[  PASSED  ] 41 tests.
```

---

### Phase 3 commit

```
refactor: Phase 3 - class level OCP refactoring (all 41 tests PASS)

Phase 3-1: add IValidator.h - pure virtual interface for validation
Phase 3-2: add CarValidator.h/cpp - concrete implementation with 5 private rule methods
Phase 3-3: add CarAssembler.h/cpp - encapsulates CarConfig state and IValidator dependency
           - main.cpp now uses CarValidator + CarAssembler instead of free functions

test: add test_phase3.cpp with 13 new tests
  P3-T1: CarValidator 6 tests (5 invalid rules + 1 valid)
  P3-T2: MockValidator + CarAssembler 7 tests (runCar/runTest/select)
```

커밋 해시: `962bf31`

---

### Phase 3 lecture_docs 파일 저장

| 파일 | 내용 |
|------|------|
| `lecture_docs/Phase3_IValidator_and_CarValidator.md` | IValidator.h + CarValidator.h/cpp 전체 소스 |
| `lecture_docs/Phase3_CarAssembler_and_test.md` | CarAssembler.h/cpp + main.cpp(Phase 3) + test_phase3.cpp 전체 소스 |

---

## 최종 리팩토링 완성 구조

```
CarAssemble/
├── car_config.h          ← enum + 상수 + CarConfig 구조체
├── IValidator.h          ← 검증 인터페이스 (OCP 핵심)
├── CarValidator.h/.cpp   ← 구체 검증 구현 (5개 규칙 캡슐화)
├── CarAssembler.h/.cpp   ← 조립 로직 + IValidator 의존성 주입
├── validator.h/.cpp      ← 자유 함수 (Phase 1/2 호환 유지)
├── car_runner.h/.cpp     ← 자유 함수 (Phase 2 호환 유지)
├── main.cpp              ← UI 루프만 (CarValidator + CarAssembler 사용)
└── test/
    ├── test_phase1.cpp   ← 17 tests (자유 함수 단위)
    ├── test_phase2.cpp   ← 11 tests (CarConfig 구조체)
    └── test_phase3.cpp   ← 13 tests (CarValidator + MockValidator)
```

**전체 테스트:** 41 PASS (Phase 1: 17 + Phase 2: 11 + Phase 3: 13)

---

## Git 커밋 히스토리 (전체 완료 시점)

| 커밋 해시 | 메시지 |
|-----------|--------|
| `fa42086` | Initial commit: CarAssemble C++ project |
| `8b51cf1` | docs: add CLAUDE.md, PLANS.md, TEST_PLANS.md for refactoring workflow |
| `206c98c` | refactor: Phase 1 - method level cleanup (all 17 tests PASS) |
| `fd26d2d` | docs: add Phase1 source snapshots to lecture_docs |
| `69bec13` | docs: add session work log to lecture_docs |
| `e7df427` | refactor: Phase 2 - module level separation + CarConfig struct (all 28 tests PASS) |
| `d6d3586` | docs: add Phase 2 source snapshots and update session work log |
| `aa4d517` | docs: split Session_WorkLog into Phase1 / Phase2 separate files |
| `962bf31` | refactor: Phase 3 - class level OCP refactoring (all 41 tests PASS) |

# CarAssemble Refactoring Plan

> 참고 문서: `docs/[CRA_AI] Day2_1_Agentic Engineering.pdf` (pp. 20–24)  
> 리팩토링 방향: 작은 변경(method level) → 큰 변경(class level) 순서로 진행

---

## 현재 코드 문제점

| 문제 | 위치 |
|------|------|
| 전역 배열 `stack[10]`으로 상태 관리 | `assemble.cpp:19` |
| 하나의 파일에 UI·비즈니스 로직·검증 혼재 | `assemble.cpp` 전체 |
| 함수명 대소문자 불일치 (`selectbrakeSystem`) | `assemble.cpp:278` |
| 매직 넘버 직접 사용 (`stack[Engine_Q] == 4`) | `assemble.cpp:334` |
| GMock 테스트 가능한 인터페이스 없음 | 전체 |

---

## Phase 1 — Method Level (함수 단위 정리)

### 1-1. 함수명 네이밍 통일
- `selectbrakeSystem` → `selectBrakeSystem`
- `testProducedCar` → `runTest`
- `runProducedCar` → `runCar`

**변경 파일:** `assemble.cpp`

---

### 1-2. 매직 넘버 제거
고장난 엔진 번호 `4`를 enum에 추가하고 직접 숫자 참조를 제거한다.

```cpp
// Before
if (stack[Engine_Q] == 4) { ... }

// After
enum Engine { GM = 1, TOYOTA, WIA, BROKEN };
if (stack[Engine_Q] == BROKEN) { ... }
```

**변경 파일:** `assemble.cpp`

---

### 1-3. 검증 함수 분리
`isValidCheck()` 내 조건을 각 규칙별 함수로 분리하여 가독성과 테스트 용이성을 높인다.

```cpp
bool isSedanContinentalConflict();   // 규칙 1
bool isSuvToyotaConflict();          // 규칙 2
bool isTruckWiaConflict();           // 규칙 3
bool isTruckMandoConflict();         // 규칙 4
bool isBoschBrakeWithoutBoschSteer();// 규칙 5
```

**변경 파일:** `assemble.cpp`

---

### 1-4. 출력 문자열 상수화
반복되는 부품명 문자열을 상수로 추출한다.

```cpp
constexpr const char* MSG_SEDAN   = "Sedan";
constexpr const char* MSG_GM      = "GM";
// ...
```

**변경 파일:** `assemble.cpp`

---

## Phase 2 — Module Level (파일 분리)

### 2-1. 파일 구조 분리

```
CarAssemble/
├── assemble.cpp        (main + UI 루프만 남김)
├── car_config.h        (enum 정의: CarType, Engine, brakeSystem, SteeringSystem)
├── validator.h / .cpp  (isValidCheck 및 규칙별 함수)
└── car_runner.h / .cpp (runCar, runTest, selectXxx 함수)
```

---

### 2-2. 전역 상태 캡슐화
`stack[10]` 전역 배열을 구조체로 감싸 상태를 명시적으로 관리한다.

```cpp
// car_config.h
struct CarConfig {
    CarType      carType;
    Engine       engine;
    brakeSystem  brake;
    SteeringSystem steering;
};
```

**변경 파일:** `car_config.h` 신규, `assemble.cpp` 수정

---

## Phase 3 — Class Level (객체 지향 설계)

### 3-1. IValidator 인터페이스 추출 (GMock 대응)

GMock으로 검증 로직을 테스트하기 위해 인터페이스를 먼저 정의한다.

```cpp
// IValidator.h
class IValidator {
public:
    virtual ~IValidator() = default;
    virtual bool isValid(const CarConfig& config) = 0;
};
```

---

### 3-2. CarValidator 클래스 구현

```cpp
// CarValidator.h / .cpp
class CarValidator : public IValidator {
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

### 3-3. CarAssembler 클래스 구현

부품 선택과 실행 로직을 담당한다.

```cpp
// CarAssembler.h / .cpp
class CarAssembler {
public:
    explicit CarAssembler(IValidator* validator);
    void selectCarType(int answer);
    void selectEngine(int answer);
    void selectBrakeSystem(int answer);
    void selectSteeringSystem(int answer);
    void runCar();
    void runTest();
private:
    CarConfig   config_;
    IValidator* validator_;
};
```

---

### 3-4. GMock 테스트 작성

`IValidator` 인터페이스를 Mock하여 `CarAssembler` 를 독립적으로 테스트한다.

```cpp
// test_car_assembler.cpp
class MockValidator : public IValidator {
public:
    MOCK_METHOD(bool, isValid, (const CarConfig&), (override));
};

TEST(CarAssemblerTest, RunCar_InvalidConfig_PrintsError) {
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));

    CarAssembler assembler(&mock);
    assembler.runCar(); // "자동차가 동작되지 않습니다" 출력 확인
}
```

---

## 진행 순서 요약

| 단계 | 작업 | 범위 |
|------|------|------|
| Phase 1-1 | 함수명 네이밍 통일 | method |
| Phase 1-2 | 매직 넘버 enum 추가 | method |
| Phase 1-3 | 검증 함수 분리 | method |
| Phase 1-4 | 출력 문자열 상수화 | method |
| Phase 2-1 | 파일 분리 | module |
| Phase 2-2 | 전역 배열 → 구조체 | module |
| Phase 3-1 | IValidator 인터페이스 | class |
| Phase 3-2 | CarValidator 구현 | class |
| Phase 3-3 | CarAssembler 구현 | class |
| Phase 3-4 | GMock 테스트 작성 | test |

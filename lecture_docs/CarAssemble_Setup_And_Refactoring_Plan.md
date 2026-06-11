# CarAssemble — 세팅 및 리팩토링 플랜 정리

> 작성일: 2026-06-11  
> 프로젝트 경로: `D:\02_Study\02_ClaudeCode\reviewer\TAKA\CarAssemble\CarAssemble\CarAssemble`

---

## 1. README.md 작성

**사용자 요청:**
> README.md 파일 작성해줘.

**작업 내용:**  
프로젝트 구조를 분석하여 `README.md`를 생성했다.

**README 주요 내용:**
- 프로젝트 개요: 자동차 부품 조합 시뮬레이터 (C++, Visual Studio 2022, Win32 Console)
- 빌드 환경 및 실행 방법
- 사용 방법: 5단계 메뉴 (차종 → 엔진 → 제동 → 조향 → 실행/테스트)
- 호환성 규칙 5가지
- 프로젝트 구조 및 주요 함수 목록

**호환성 규칙 요약:**

| 규칙 | 내용 |
|------|------|
| 1 | Sedan은 CONTINENTAL 브레이크 사용 불가 |
| 2 | SUV는 TOYOTA 엔진 사용 불가 |
| 3 | Truck은 WIA 엔진 사용 불가 |
| 4 | Truck은 MANDO 브레이크 사용 불가 |
| 5 | BOSCH 브레이크는 BOSCH 조향 시스템만 사용 가능 |

---

## 2. Visual Studio에서 GMock 세팅하는 방법

**사용자 요청:**
> Visual Studio에서 GMOCK 세팅하는 방법 순서대로 알려줘.

**방법 1 — NuGet 패키지 (가장 간단)**

```
메뉴 → 도구 → NuGet 패키지 관리자 → 솔루션용 NuGet 패키지 관리
→ "googletest" 검색
→ Microsoft.googletest.v140.windesktop.msvcstl.dyn.rt-dyn 선택
→ 프로젝트 체크 후 설치
```

**방법 2 — vcpkg (권장)**

```powershell
git clone https://github.com/microsoft/vcpkg C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
C:\vcpkg\vcpkg integrate install
C:\vcpkg\vcpkg install gtest:x64-windows
```

**프로젝트 속성 설정 (vcpkg 수동 설정 시):**

| 항목 | 설정값 |
|------|--------|
| C/C++ → 추가 포함 디렉터리 | `C:\vcpkg\installed\x64-windows\include` |
| 링커 → 추가 라이브러리 디렉터리 | `C:\vcpkg\installed\x64-windows\debug\lib` (Debug) |
| 링커 → 추가 종속성 | `gmock.lib`, `gmock_main.lib`, `gtest.lib`, `gtest_main.lib` |
| C/C++ → 런타임 라이브러리 | Debug: `/MDd` / Release: `/MD` |

**자주 발생하는 오류:**

| 오류 | 원인 | 해결 |
|------|------|------|
| `LNK2019` | .lib 경로 미설정 | 링커 추가 라이브러리 디렉터리 확인 |
| `LNK2038` | 런타임 불일치 `/MT` vs `/MD` | 프로젝트와 gmock 런타임 일치 |
| 헤더 `not found` | include 경로 미설정 | 추가 포함 디렉터리 확인 |

---

## 3. 기존 CarAssemble 프로젝트에 GMock 연결

**사용자 요청:**
> 이미 프로젝트를 CarAssemble이라고 해서 만들었어. 여기에 GMOCK을 설정하고 싶어 어떻게 해야해?

**핵심 발견:**  
`assemble.cpp` 상단에 이미 `#ifdef _DEBUG` 블록으로 GMock 코드가 준비되어 있었다.

```cpp
#ifdef _DEBUG
    #include "gmock/gmock.h"
    int main() {
        testing::InitGoogleMock();
        return RUN_ALL_TESTS();
    }
#else
    // 자동차 조립 시뮬레이터 코드
    int main() { ... }
#endif
```

- **Debug 빌드** → GMock 테스트 실행
- **Release 빌드** → 자동차 조립 시뮬레이터 실행

**NuGet으로 설치 (사용자 확인):**

```
메뉴 → 도구 → NuGet 패키지 관리자 → 솔루션용 NuGet 패키지 관리
→ "googletest" 검색
→ Microsoft.googletest.v140.windesktop.msvcstl.dyn.rt-dyn 선택
→ CarAssemble 프로젝트 체크 후 설치
```

NuGet 설치 시 `.vcxproj`에 include/lib 경로가 자동 추가된다.

**설치 후 런타임 라이브러리 확인:**
```
프로젝트 속성 → C/C++ → 코드 생성 → 런타임 라이브러리
  Debug   → /MDd
  Release → /MD
```

**정상 실행 확인:**
```
[==========] Running 0 tests from 0 test suites.
[  PASSED  ] 0 tests.
```

---

## 4. 리팩토링 플랜

**사용자 요청:**
> 지금 이 프로젝트를 refactoring 할 건데, 이와 관련된 내용은 `docs/[CRA_AI] Day2_1_Agentic Engineering.pdf` 의 20~24 page에서 확인할 수 있다. 이와 관련하여 plan을 세우고, temp_docs/PLAN.md를 작성하되 refactoring plan은 작은 변경(method level)부터 큰 변경 순서(class level)로 진행 해줘.

**참고 문서:** `docs/[CRA_AI] Day2_1_Agentic Engineering.pdf` pp. 20–24  
**산출물:** `temp_docs/PLAN.md`

---

### 현재 코드 문제점

| 문제 | 위치 |
|------|------|
| 전역 배열 `stack[10]`으로 상태 관리 | `assemble.cpp:19` |
| 하나의 파일에 UI·비즈니스 로직·검증 혼재 | `assemble.cpp` 전체 |
| 함수명 대소문자 불일치 (`selectbrakeSystem`) | `assemble.cpp:278` |
| 매직 넘버 직접 사용 (`stack[Engine_Q] == 4`) | `assemble.cpp:334` |
| GMock 테스트 가능한 인터페이스 없음 | 전체 |

---

### Phase 1 — Method Level

| 작업 | 내용 |
|------|------|
| 1-1. 함수명 네이밍 통일 | `selectbrakeSystem` → `selectBrakeSystem`, `testProducedCar` → `runTest` 등 |
| 1-2. 매직 넘버 제거 | 고장난 엔진 `4` → `BROKEN` enum 추가 |
| 1-3. 검증 함수 분리 | `isValidCheck()` 내 5개 규칙을 각각 별도 함수로 분리 |
| 1-4. 출력 문자열 상수화 | 반복 문자열 → `constexpr const char*` 상수 추출 |

**1-2 변경 예시:**
```cpp
// Before
if (stack[Engine_Q] == 4) { ... }

// After
enum Engine { GM = 1, TOYOTA, WIA, BROKEN };
if (stack[Engine_Q] == BROKEN) { ... }
```

**1-3 분리 함수 예시:**
```cpp
bool isSedanContinentalConflict();    // 규칙 1
bool isSuvToyotaConflict();           // 규칙 2
bool isTruckWiaConflict();            // 규칙 3
bool isTruckMandoConflict();          // 규칙 4
bool isBoschBrakeWithoutBoschSteer(); // 규칙 5
```

---

### Phase 2 — Module Level

| 작업 | 내용 |
|------|------|
| 2-1. 파일 분리 | `car_config.h`, `validator.h/.cpp`, `car_runner.h/.cpp` |
| 2-2. 전역 상태 캡슐화 | `stack[10]` → `CarConfig` 구조체 |

**파일 구조 목표:**
```
CarAssemble/
├── assemble.cpp        (main + UI 루프)
├── car_config.h        (enum 및 CarConfig 구조체)
├── validator.h / .cpp  (검증 로직)
└── car_runner.h / .cpp (부품 선택 및 실행)
```

**CarConfig 구조체:**
```cpp
struct CarConfig {
    CarType        carType;
    Engine         engine;
    brakeSystem    brake;
    SteeringSystem steering;
};
```

---

### Phase 3 — Class Level

| 작업 | 내용 |
|------|------|
| 3-1. IValidator 인터페이스 추출 | GMock 테스트를 위한 핵심 인터페이스 |
| 3-2. CarValidator 클래스 구현 | IValidator 구현체 |
| 3-3. CarAssembler 클래스 구현 | 부품 선택 + 실행 로직 |
| 3-4. GMock 테스트 작성 | MockValidator로 CarAssembler 독립 테스트 |

**IValidator 인터페이스:**
```cpp
class IValidator {
public:
    virtual ~IValidator() = default;
    virtual bool isValid(const CarConfig& config) = 0;
};
```

**CarAssembler 클래스:**
```cpp
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

**GMock 테스트 예시:**
```cpp
class MockValidator : public IValidator {
public:
    MOCK_METHOD(bool, isValid, (const CarConfig&), (override));
};

TEST(CarAssemblerTest, RunCar_InvalidConfig_PrintsError) {
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));
    CarAssembler assembler(&mock);
    assembler.runCar();
}
```

---

### 전체 진행 순서

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

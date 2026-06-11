# CarAssemble 세션 작업 기록

---

## 1. OCP 적용 리팩토링 플랜 수립

**사용자 요청:**
> Car_type, Engine_type, brake_system, steering_substem 의 제약조건등을 적용 시켜셔 OCP가 적용된 확장성 높은 구조를 refactoring 되도록 다시 plan을 세워봐.

**현재 코드 분석 결과:**

`main.cpp` 단일 파일, 완전 절차적 구조

```
enum QuestionType { CarType_Q, Engine_Q, brakeSystem_Q, SteeringSystem_Q, Run_Test };
enum CarType      { SEDAN=1, SUV, TRUCK };
enum Engine       { GM=1, TOYOTA, WIA };          // 4 = 고장난 엔진 (매직 넘버)
enum brakeSystem  { MANDO=1, CONTINENTAL, BOSCH_B };
enum SteeringSystem { BOSCH_S=1, MOBIS };
```

전역 배열 `int stack[10]` 으로 상태 관리.

**5개 하드코딩 제약조건 (`isValidCheck()` 내 if-else 체인):**

```
1. SEDAN    + CONTINENTAL Brake  = INVALID
2. SUV      + TOYOTA Engine      = INVALID
3. TRUCK    + WIA Engine         = INVALID
4. TRUCK    + MANDO Brake        = INVALID
5. BOSCH Brake → BOSCH Steering 필수
```

동일 조건이 `isValidCheck()`와 `testProducedCar()` 두 곳에 중복.

---

## 2. GitHub Remote Repository 연결

**사용자 요청:**
> https://github.com/gaihwan/CarAssembleTAKA.git에 지금 작업하고 있는 코드를 연동하기 위한 github remote repository를 만들었어. 현재 CarAssemble Project를 이 git hub에 clone을 하여 연결해줘.

**실행 순서:**

```bash
# 1. .gitignore 생성 (VS 빌드 산출물, .claude/ 제외)
# 2. git init
git init
# 3. remote 추가
git remote add origin https://github.com/gaihwan/CarAssembleTAKA.git
# 4. 파일 스테이징
git add main.cpp CarAssemble.vcxproj CarAssemble.vcxproj.filters packages.config README.md .gitignore docs/ lecture_docs/
# 5. 초기 커밋
git commit -m "Initial commit: CarAssemble C++ project"
# 6. 푸시
git push -u origin master
```

**`.gitignore` 내용:**

```
# Visual Studio build output
Debug/
Release/
x64/
x86/
*.user
*.suo
*.aps
*.ncb
*.sdf
*.opensdf
*.VC.db
*.VC.VC.opendb
.vs/

# Compiled object files
*.obj
*.pdb
*.ilk
*.exp
*.lib
*.dll
*.exe

# NuGet packages
packages/

# Claude Code internal files
.claude/
```

**푸시된 파일:**
- `main.cpp`
- `CarAssemble.vcxproj`
- `CarAssemble.vcxproj.filters`
- `packages.config`
- `README.md`
- `docs/[CRA_AI] Day2_1_Agentic Engineering.pdf`
- `lecture_docs/CarAssemble_Setup_And_Refactoring_Plan.md`
- `.gitignore`

---

## 3. 현재 상태 커밋 확인

**사용자 요청:**
> 그럼 현재 상태로 커밋을 한번 해줘.

**결과:**

```
On branch master
Your branch is up to date with 'origin/master'.
nothing to commit, working tree clean
```

초기 커밋에서 모든 파일이 이미 포함되어 있어 변경사항 없음.

---

## 4. PLANS.md / TEST_PLANS.md / CLAUDE.md 작성

**사용자 요청:**
> 지금 이 프로젝트를 refactoring 할 건데, 이와 관련된 내용은 이미 temp_dos/PLAN.md에 작성되어 있어. 이 파일 이름은 PLANS.md로 변경해줘. 이 PLANS.md 파일은 CLAUDE.md에 연결해줘.
> 그리고 이 plan대로 refactoring 하기 위해, gmoc으로 기본 세팅되어 있는 unit test를 이용하여, refactoring 관련 unit test 수행 계획도 temp_docs/TEST_PLANS.md로 작성해주고 이 또한 CLAUDE.md에 연결해줘. 이를 이용하여 앞으로, PLAN에 나와있는 Phase 단계에 따라 코드를 refacotring하고, unit test를 진행한후, 문제가 없으면, commit을 하는 순서로 진행할 거야. 일단 PLANS.md와 연계된 unit test 계획부터 세워줘.

**.gitignore 수정** (temp_docs/ 제외 항목 삭제 → git 추적 대상으로 변경):

```diff
- # Claude Code internal files
- .claude/
- temp_docs/
+ # Claude Code internal files
+ .claude/
```

**파일명 변경:**

```bash
cp temp_docs/PLAN.md temp_docs/PLANS.md
rm temp_docs/PLAN.md
```

---

### 4-1. temp_docs/TEST_PLANS.md 전체 내용

```markdown
# CarAssemble Unit Test Plan (GMock)

> 참조: `temp_docs/PLANS.md`
> 테스트 프레임워크: GMock 1.11.0 (NuGet)
> 원칙: 각 Phase 리팩토링 완료 후 해당 Phase 테스트 통과 확인 → commit

## 테스트 파일 구조

CarAssemble/
├── test/
│   ├── test_phase1.cpp   ← Phase 1 함수 단위 테스트
│   ├── test_phase2.cpp   ← Phase 2 모듈/구조체 테스트
│   └── test_phase3.cpp   ← Phase 3 클래스 + GMock 테스트
└── ...

## Phase 1 테스트 — Method Level

### [P1-T1] BROKEN 엔진 enum 등록 확인 (Phase 1-2)

TEST(EnumTest, BrokenEngine_IsDefinedAsValue4) {
    EXPECT_EQ(static_cast<int>(BROKEN), 4);
}

### [P1-T2] 각 규칙 함수 — 충돌 감지 (Phase 1-3)

// 규칙 1
TEST(RuleTest, SedanContinental_DetectsConflict) { ... }
TEST(RuleTest, SedanMando_NoConflict)            { ... }
// 규칙 2
TEST(RuleTest, SuvToyota_DetectsConflict)        { ... }
TEST(RuleTest, SuvGM_NoConflict)                 { ... }
// 규칙 3
TEST(RuleTest, TruckWia_DetectsConflict)         { ... }
// 규칙 4
TEST(RuleTest, TruckMando_DetectsConflict)       { ... }
// 규칙 5
TEST(RuleTest, BoschBrakeWithoutBoschSteer_DetectsConflict) { ... }
TEST(RuleTest, BoschBrakeWithBoschSteer_NoConflict)         { ... }

### [P1-T3] isValidCheck() 종합 (Phase 1-3)

TEST(ValidCheckTest, AllValid_ReturnsTrue)                  { ... }
TEST(ValidCheckTest, Rule1_SedanContinental_ReturnsFalse)   { ... }
TEST(ValidCheckTest, Rule2_SuvToyota_ReturnsFalse)          { ... }
TEST(ValidCheckTest, Rule3_TruckWia_ReturnsFalse)           { ... }
TEST(ValidCheckTest, Rule4_TruckMando_ReturnsFalse)         { ... }
TEST(ValidCheckTest, Rule5_BoschBrakeWithMobis_ReturnsFalse){ ... }

## Phase 2 테스트 — Module Level

### [P2-T1] CarConfig 구조체 기본 초기화 (Phase 2-2)

TEST(CarConfigTest, DefaultInitialization) { ... }

### [P2-T2] isValidCheck(CarConfig) 오버로드 — 규칙별 검증 (Phase 2-2)

TEST(CarConfigValidationTest, Rule1_SedanContinental_ReturnsFalse) { ... }
TEST(CarConfigValidationTest, Rule2_SuvToyota_ReturnsFalse)        { ... }
TEST(CarConfigValidationTest, Rule3_TruckWia_ReturnsFalse)         { ... }
TEST(CarConfigValidationTest, Rule4_TruckMando_ReturnsFalse)       { ... }
TEST(CarConfigValidationTest, Rule5_BoschBrakeWithoutBoschSteer_ReturnsFalse) { ... }
TEST(CarConfigValidationTest, AllRulesPass_ReturnsTrue)            { ... }

## Phase 3 테스트 — Class Level (GMock)

### [P3-T1] CarValidator — 각 규칙 메서드 단위 테스트 (Phase 3-2)

class CarValidatorTest : public ::testing::Test {
protected:
    CarValidator validator;
};

TEST_F(CarValidatorTest, SedanContinental_IsInvalid) { ... }
TEST_F(CarValidatorTest, SuvToyota_IsInvalid)        { ... }
TEST_F(CarValidatorTest, TruckWia_IsInvalid)         { ... }
TEST_F(CarValidatorTest, TruckMando_IsInvalid)       { ... }
TEST_F(CarValidatorTest, BoschBrakeWithoutBoschSteer_IsInvalid) { ... }
TEST_F(CarValidatorTest, ValidConfig_ReturnsTrue)    { ... }

### [P3-T2] MockValidator로 CarAssembler 독립 테스트 (Phase 3-3)

class MockValidator : public IValidator {
public:
    MOCK_METHOD(bool, isValid, (const CarConfig&), (override));
};

// runCar() 테스트
TEST(CarAssemblerTest, RunCar_ValidConfig_PrintsRunning)  { ... }
TEST(CarAssemblerTest, RunCar_InvalidConfig_PrintsError)  { ... }
TEST(CarAssemblerTest, RunCar_BrokenEngine_PrintsBroken)  { ... }
// runTest() 테스트
TEST(CarAssemblerTest, RunTest_ValidConfig_PrintsPass)    { ... }
TEST(CarAssemblerTest, RunTest_InvalidConfig_PrintsFail)  { ... }
// 선택 저장 확인
TEST(CarAssemblerTest, SelectCarType_StoresCorrectly)     { ... }
TEST(CarAssemblerTest, SelectEngine_StoresCorrectly)      { ... }

## Phase별 테스트 통과 기준 & Commit 조건

| Phase   | 테스트 파일      | 통과 기준           | Commit 메시지 예시                                |
|---------|-----------------|---------------------|---------------------------------------------------|
| Phase 1 | test_phase1.cpp | P1-T1, T2, T3 통과  | refactor: add BROKEN enum, extract rule functions |
| Phase 2 | test_phase2.cpp | P2-T1, T2 통과      | refactor: replace stack[] with CarConfig struct   |
| Phase 3 | test_phase3.cpp | P3-T1, T2 통과      | refactor: implement CarValidator, CarAssembler    |

## GMock 테스트 프로젝트 설정 (Visual Studio)

1. 솔루션에 새 프로젝트 추가: 빈 프로젝트 (C++) → 이름: CarAssembleTest
2. NuGet 패키지 설치: gmock 1.11.0
3. 프로젝트 참조: CarAssemble 프로젝트의 헤더 경로 추가
4. test/ 폴더의 .cpp 파일을 프로젝트에 포함
5. main 진입점:

// test_main.cpp
#include <gtest/gtest.h>
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
```

---

### 4-2. CLAUDE.md 전체 내용

```markdown
# CarAssemble Project — Claude Instructions

## 프로젝트 개요
Visual Studio C++ 자동차 조립 선택 프로그램.
현재 절차적 단일 파일(main.cpp) 구조를 OOP + GMock 테스트 가능한 구조로 리팩토링 진행 중.

## 핵심 문서

| 문서           | 경로                    | 용도                             |
|----------------|-------------------------|----------------------------------|
| 리팩토링 계획  | temp_docs/PLANS.md      | Phase별 리팩토링 작업 정의       |
| 테스트 계획    | temp_docs/TEST_PLANS.md | Phase별 GMock 단위 테스트 케이스 |

## 작업 진행 규칙

각 Phase는 반드시 아래 순서로 진행한다:

1. PLANS.md 해당 Phase 내용대로 코드 리팩토링
2. TEST_PLANS.md 해당 Phase 테스트 실행
3. 모든 테스트 PASS → git commit
4. 다음 Phase로 이동

## Phase 진행 현황

| Phase     | 내용                            | 상태 |
|-----------|---------------------------------|------|
| Phase 1-1 | 함수명 네이밍 통일               | 대기 |
| Phase 1-2 | 매직 넘버 BROKEN enum 추가       | 대기 |
| Phase 1-3 | 규칙별 검증 함수 분리            | 대기 |
| Phase 1-4 | 출력 문자열 상수화               | 대기 |
| Phase 2-1 | 파일 분리 (validator, car_runner) | 대기 |
| Phase 2-2 | 전역 stack[] → CarConfig 구조체  | 대기 |
| Phase 3-1 | IValidator 인터페이스 추출       | 대기 |
| Phase 3-2 | CarValidator 클래스 구현         | 대기 |
| Phase 3-3 | CarAssembler 클래스 구현         | 대기 |

## 주요 파일
- main.cpp — 현재 전체 소스 (리팩토링 대상)
- packages.config — gmock 1.11.0 NuGet 패키지 등록됨
- CarAssemble.vcxproj — Visual Studio 프로젝트 파일

## 제약 사항
- C++ 표준: Visual Studio 기본 설정 유지
- 테스트 없이 commit 금지
- Phase 순서 준수 (method → module → class 순)
```

---

### 4-3. commit

```
docs: add CLAUDE.md, PLANS.md, TEST_PLANS.md for refactoring workflow

- Rename PLAN.md → PLANS.md
- Add TEST_PLANS.md with GMock test cases per refactoring phase
- Add CLAUDE.md linking plans and defining phase-by-phase workflow
- Update .gitignore to track temp_docs/
```

---

## 5. Phase 1 리팩토링 실행

**사용자 요청:**
> unit test와 연계된 plan에 따른 refactoring 처음 단계 시작해줘.

**작업 흐름:** Phase 1-1 → 1-2 → 1-3 → 1-4 → 테스트 빌드 → 17/17 PASS → commit

---

### Phase 1-1: 함수명 네이밍 통일

| 변경 전 | 변경 후 |
|---------|---------|
| `selectbrakeSystem` | `selectBrakeSystem` |
| `runProducedCar` | `runCar` |
| `testProducedCar` | `runTest` |

변경 위치: 전방 선언, 함수 정의, 호출부 모두 일괄 변경

---

### Phase 1-2: 매직 넘버 제거

```cpp
// Before
enum Engine { GM = 1, TOYOTA, WIA };
if (stack[Engine_Q] == 4) { ... }

// After
enum Engine { GM = 1, TOYOTA, WIA, BROKEN };
if (stack[Engine_Q] == BROKEN) { ... }
```

---

### Phase 1-3: 검증 규칙 함수 분리

`isValidCheck()` 내부 조건을 5개 함수로 추출:

```cpp
bool isSedanContinentalConflict()
{
    return stack[CarType_Q] == SEDAN && stack[brakeSystem_Q] == CONTINENTAL;
}

bool isSuvToyotaConflict()
{
    return stack[CarType_Q] == SUV && stack[Engine_Q] == TOYOTA;
}

bool isTruckWiaConflict()
{
    return stack[CarType_Q] == TRUCK && stack[Engine_Q] == WIA;
}

bool isTruckMandoConflict()
{
    return stack[CarType_Q] == TRUCK && stack[brakeSystem_Q] == MANDO;
}

bool isBoschBrakeWithoutBoschSteer()
{
    return stack[brakeSystem_Q] == BOSCH_B && stack[SteeringSystem_Q] != BOSCH_S;
}
```

`isValidCheck()` 리팩토링:

```cpp
// Before: 5개 if-else 체인
// After:
bool isValidCheck()
{
    if (isSedanContinentalConflict())    return false;
    if (isSuvToyotaConflict())           return false;
    if (isTruckWiaConflict())            return false;
    if (isTruckMandoConflict())          return false;
    if (isBoschBrakeWithoutBoschSteer()) return false;
    return true;
}
```

`runTest()` 도 동일하게 규칙 함수 기반으로 교체 (중복 조건 제거):

```cpp
void runTest()
{
    if (isSedanContinentalConflict())
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Sedan에는 Continental제동장치 사용 불가\n");
    }
    else if (isSuvToyotaConflict())
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("SUV에는 TOYOTA엔진 사용 불가\n");
    }
    else if (isTruckWiaConflict())
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 WIA엔진 사용 불가\n");
    }
    else if (isTruckMandoConflict())
    {
        printf("자동차 부품 조합 테스트 결과 : FAIL\n");
        printf("Truck에는 Mando제동장치 사용 불가\n");
    }
    else if (isBoschBrakeWithoutBoschSteer())
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

---

### Phase 1-4: 출력 문자열 상수화

```cpp
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
```

모든 `printf` 리터럴 문자열을 상수로 교체. 예:

```cpp
// Before
printf("차량 타입으로 Sedan을 선택하셨습니다.\n");
// After
printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_SEDAN);
```

---

### main.cpp 구조 재편

테스트 빌드가 함수를 참조할 수 있도록 `#ifdef _DEBUG` / `#else` 구조를 재편:

```
[변경 전]
#ifdef _DEBUG
  → gmock main()
#else
  → 모든 enum, 전역변수, 함수 정의
  → UI main()
#endif

[변경 후]
#include (항상)
#ifdef _DEBUG → #include "gmock/gmock.h" (#endif)

constexpr 상수    (항상 컴파일)
enum 정의         (항상 컴파일)
int stack[10]     (항상 컴파일)
전방 선언         (항상 컴파일)
모든 함수 정의    (항상 컴파일)

#ifdef _DEBUG
  → int main() { InitGoogleMock(); RUN_ALL_TESTS(); }
#else
  → int main() { UI 루프 }
#endif
```

→ Debug 빌드: GMock 테스트 러너 실행  
→ Release 빌드: UI 루프 실행

---

### test/test_phase1.cpp 작성

`CarAssemble.vcxproj`에 Debug 전용으로 추가:

```xml
<ClCompile Include="test\test_phase1.cpp">
  <ExcludedFromBuild Condition="'$(Configuration)'=='Release'">true</ExcludedFromBuild>
</ClCompile>
```

`test_phase1.cpp` 구조:

```cpp
#include <gtest/gtest.h>

extern int stack[];

// main.cpp 와 값 동일한 enum 선언
enum QuestionType { CarType_Q=0, Engine_Q=1, brakeSystem_Q=2, SteeringSystem_Q=3, Run_Test=4 };
enum CarType      { SEDAN=1, SUV=2, TRUCK=3 };
enum Engine       { GM=1, TOYOTA=2, WIA=3, BROKEN=4 };
enum brakeSystem  { MANDO=1, CONTINENTAL=2, BOSCH_B=3 };
enum SteeringSystem { BOSCH_S=1, MOBIS=2 };

extern bool isSedanContinentalConflict();
extern bool isSuvToyotaConflict();
extern bool isTruckWiaConflict();
extern bool isTruckMandoConflict();
extern bool isBoschBrakeWithoutBoschSteer();
extern bool isValidCheck();

// [P1-T1] BROKEN enum 값 = 4
// [P1-T2] 5개 규칙 함수 개별 — 충돌/비충돌 케이스
// [P1-T3] isValidCheck() 종합 — 6개 케이스
```

---

### 빌드 및 테스트 실행

```
MSBuild CarAssemble.sln /p:Configuration=Debug /p:Platform=x64

빌드 결과: 성공 (경고: C4819 코드 페이지 — 한글 포함, 무해)
```

```
실행: x64\Debug\CarAssemble.exe

[==========] Running 17 tests from 3 test suites.
[----------] 1 test from EnumTest
[ RUN      ] EnumTest.BrokenEngine_IsDefinedAsValue4       [OK]
[----------] 10 tests from RuleTest
[ RUN      ] RuleTest.SedanContinental_DetectsConflict     [OK]
[ RUN      ] RuleTest.SedanMando_NoConflict                [OK]
[ RUN      ] RuleTest.SuvToyota_DetectsConflict            [OK]
[ RUN      ] RuleTest.SuvGM_NoConflict                     [OK]
[ RUN      ] RuleTest.TruckWia_DetectsConflict             [OK]
[ RUN      ] RuleTest.TruckGM_NoConflict                   [OK]
[ RUN      ] RuleTest.TruckMando_DetectsConflict           [OK]
[ RUN      ] RuleTest.TruckBosch_NoConflict                [OK]
[ RUN      ] RuleTest.BoschBrakeWithMobis_DetectsConflict  [OK]
[ RUN      ] RuleTest.BoschBrakeWithBoschSteer_NoConflict  [OK]
[----------] 6 tests from ValidCheckTest
[ RUN      ] ValidCheckTest.AllValid_ReturnsTrue                    [OK]
[ RUN      ] ValidCheckTest.Rule1_SedanContinental_ReturnsFalse     [OK]
[ RUN      ] ValidCheckTest.Rule2_SuvToyota_ReturnsFalse            [OK]
[ RUN      ] ValidCheckTest.Rule3_TruckWia_ReturnsFalse             [OK]
[ RUN      ] ValidCheckTest.Rule4_TruckMando_ReturnsFalse           [OK]
[ RUN      ] ValidCheckTest.Rule5_BoschBrakeWithMobis_ReturnsFalse  [OK]
[==========] 17 tests from 3 test suites ran. (4 ms total)
[  PASSED  ] 17 tests.
```

---

### Phase 1 commit

```
refactor: Phase 1 - method level cleanup (all 17 tests PASS)

Phase 1-1: rename selectbrakeSystem→selectBrakeSystem, runProducedCar→runCar, testProducedCar→runTest
Phase 1-2: add BROKEN to Engine enum, remove magic number 4
Phase 1-3: extract 5 rule functions (isSedanContinentalConflict etc.), refactor isValidCheck/runTest to use them
Phase 1-4: constexpr string constants for all part names

test/test_phase1.cpp: 17 GMock tests covering P1-T1, P1-T2, P1-T3 — all PASS
```

---

## 6. lecture_docs 파일 저장 요청 (1차)

**사용자 요청:**
> Phase1 에서 작업한 내용을 raw 파일 내용 그대로 lecture_docs 폴더에 적당한 이름으로 md 파일로 저장해줘.

**처리 결과:** `main.cpp`, `test/test_phase1.cpp` 파일 내용을 코드 블록으로 감싸서 저장

- `lecture_docs/Phase1_main.md`
- `lecture_docs/Phase1_test.md`

---

## 7. lecture_docs 파일 저장 요청 (2차 — 본 파일)

**사용자 요청:**
> 아니. 이 창을 통해 작업한 내용을 그대로 순서대로 정리(요약은 가능한 하지 말고...) 해서 md 파일로 저장해달라는 요청이었어.

**처리 결과:** `lecture_docs/Session_WorkLog.md` 생성 (세션 전체 작업 내용 순서대로 기록)

---

## 8. 사용자 요청 포함 재저장

**사용자 요청:**
> 내가 입력한 요청사항도 같이 추가해서 저장해줘.

**처리 결과:** `Session_WorkLog.md` 전체를 사용자 요청 인용문 포함하여 재작성

---

## Git 커밋 히스토리 (세션 종료 시점)

| 커밋 해시 | 메시지 |
|-----------|--------|
| `fa42086` | Initial commit: CarAssemble C++ project |
| `8b51cf1` | docs: add CLAUDE.md, PLANS.md, TEST_PLANS.md for refactoring workflow |
| `206c98c` | refactor: Phase 1 - method level cleanup (all 17 tests PASS) |
| `fd26d2d` | docs: add Phase1 source snapshots to lecture_docs |
| `69bec13` | docs: add session work log to lecture_docs |

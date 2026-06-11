# CarAssemble

자동차 부품 조합 시뮬레이터 — 차종, 엔진, 제동 시스템, 조향 시스템을 선택하고 호환성 규칙을 검증하는 콘솔 애플리케이션입니다.

## 개요

사용자가 단계별 메뉴를 통해 자동차를 조립하고, 부품 간 호환성을 확인하거나 조립된 차를 실행할 수 있습니다.

## 빌드 환경

- 언어: C++
- IDE: Visual Studio 2022 (v17 이상)
- 플랫폼: Win32 / x64 Console Application
- 빌드 시스템: MSBuild

## 빌드 및 실행

Visual Studio에서 `CarAssemble.sln`을 열고 **빌드(F7)** 후 **실행(F5)** 합니다.

## 사용 방법

실행하면 다음 순서로 부품을 선택합니다.

```
1. 차종 선택   → Sedan / SUV / Truck
2. 엔진 선택   → GM / TOYOTA / WIA
3. 제동 시스템 → MANDO / CONTINENTAL / BOSCH
4. 조향 시스템 → BOSCH / MOBIS
5. 동작 선택   → RUN(실행) / TEST(테스트) / 처음부터
```

각 단계에서 `0`을 입력하면 이전 단계로 돌아갑니다.

## 호환성 규칙

| 규칙 | 내용 |
|------|------|
| 1 | Sedan은 CONTINENTAL 브레이크 사용 불가 |
| 2 | SUV는 TOYOTA 엔진 사용 불가 |
| 3 | Truck은 WIA 엔진 사용 불가 |
| 4 | Truck은 MANDO 브레이크 사용 불가 |
| 5 | BOSCH 브레이크는 BOSCH 조향 시스템만 사용 가능 |

## 프로젝트 구조

```
CarAssemble/
├── CarAssemble.sln          # Visual Studio 솔루션
└── CarAssemble/
    ├── assemble.cpp         # 전체 애플리케이션 로직
    ├── CarAssemble.vcxproj  # 프로젝트 설정
    └── .claude/
        └── settings.local.json
```

## 주요 함수

| 함수 | 역할 |
|------|------|
| `main()` | 메뉴 루프, 입력 처리, 화면 전환 |
| `selectCarType()` | 차종 선택 및 기록 |
| `selectEngine()` | 엔진 선택 및 기록 |
| `selectbrakeSystem()` | 제동 시스템 선택 및 기록 |
| `selectSteeringSystem()` | 조향 시스템 선택 및 기록 |
| `isValidCheck()` | 호환성 규칙 검증 |
| `runProducedCar()` | 조립된 차 실행 및 구성 출력 |
| `testProducedCar()` | 호환성 테스트 및 PASS/FAIL 출력 |

# CarAssemble Project — Claude Instructions

## 프로젝트 개요
Visual Studio C++ 자동차 조립 선택 프로그램.
절차적 단일 파일(`main.cpp`) 구조에서 OOP + GMock 테스트 가능한 구조로 리팩토링 완료.

## 핵심 문서

| 문서 | 경로 | 용도 |
|------|------|------|
| 리팩토링 계획 | `temp_docs/PLANS.md` | Phase별 리팩토링 작업 정의 |
| 테스트 계획 | `temp_docs/TEST_PLANS.md` | Phase별 GMock 단위 테스트 케이스 |

## 작업 진행 규칙

각 Phase는 반드시 아래 순서로 진행한다:

```
1. PLANS.md 해당 Phase 내용대로 코드 리팩토링
2. TEST_PLANS.md 해당 Phase 테스트 실행
3. 모든 테스트 PASS → git commit
4. 다음 Phase로 이동
```

## Phase 진행 현황

| Phase | 내용 | 상태 |
|-------|------|------|
| Phase 1-1 | 함수명 네이밍 통일 | 완료 |
| Phase 1-2 | 매직 넘버 BROKEN enum 추가 | 완료 |
| Phase 1-3 | 규칙별 검증 함수 분리 | 완료 |
| Phase 1-4 | 출력 문자열 상수화 | 완료 |
| Phase 2-1 | 파일 분리 (validator, car_runner) | 완료 |
| Phase 2-2 | 전역 stack[] → CarConfig 구조체 | 완료 |
| Phase 3-1 | IValidator 인터페이스 추출 | 완료 |
| Phase 3-2 | CarValidator 클래스 구현 | 완료 |
| Phase 3-3 | CarAssembler 클래스 구현 | 완료 |

**전체 테스트: 41/41 PASS** (Phase 1: 17 + Phase 2: 11 + Phase 3: 13)

## 현재 파일 구조

```
CarAssemble/
├── car_config.h          ← enum + 상수 + CarConfig 구조체
├── IValidator.h          ← 검증 인터페이스 (OCP 핵심)
├── CarValidator.h/.cpp   ← 구체 검증 구현 (5개 규칙 캡슐화)
├── CarAssembler.h/.cpp   ← 조립 로직 + IValidator 의존성 주입
├── validator.h/.cpp      ← 자유 함수 (Phase 1/2 호환 유지)
├── car_runner.h/.cpp     ← 자유 함수 (Phase 2 호환 유지)
├── main.cpp              ← UI 루프 (CarValidator + CarAssembler 사용)
├── packages.config       ← gmock 1.11.0 NuGet 패키지
├── CarAssemble.vcxproj   ← Visual Studio 프로젝트 파일
└── test/
    ├── test_phase1.cpp   ← 17 tests
    ├── test_phase2.cpp   ← 11 tests
    └── test_phase3.cpp   ← 13 tests
```

## 제약 사항
- C++ 표준: Visual Studio 기본 설정 유지
- 테스트 없이 commit 금지
- Phase 순서 준수 (method → module → class 순)

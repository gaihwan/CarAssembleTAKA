# CarAssemble Project — Claude Instructions

## 프로젝트 개요
Visual Studio C++ 자동차 조립 선택 프로그램.  
현재 절차적 단일 파일(`main.cpp`) 구조를 OOP + GMock 테스트 가능한 구조로 리팩토링 진행 중.

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
| Phase 1-1 | 함수명 네이밍 통일 | 대기 |
| Phase 1-2 | 매직 넘버 BROKEN enum 추가 | 대기 |
| Phase 1-3 | 규칙별 검증 함수 분리 | 대기 |
| Phase 1-4 | 출력 문자열 상수화 | 대기 |
| Phase 2-1 | 파일 분리 (validator, car_runner) | 대기 |
| Phase 2-2 | 전역 stack[] → CarConfig 구조체 | 대기 |
| Phase 3-1 | IValidator 인터페이스 추출 | 대기 |
| Phase 3-2 | CarValidator 클래스 구현 | 대기 |
| Phase 3-3 | CarAssembler 클래스 구현 | 대기 |

## 주요 파일
- `main.cpp` — 현재 전체 소스 (리팩토링 대상)
- `packages.config` — gmock 1.11.0 NuGet 패키지 등록됨
- `CarAssemble.vcxproj` — Visual Studio 프로젝트 파일

## 제약 사항
- C++ 표준: Visual Studio 기본 설정 유지
- 테스트 없이 commit 금지
- Phase 순서 준수 (method → module → class 순)

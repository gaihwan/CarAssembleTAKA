// CarAssemble - main.cpp
// Phase 1 refactoring: naming, BROKEN enum, rule functions, string constants

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _DEBUG
#include "gmock/gmock.h"
#endif

#define CLEAR_SCREEN "\033[H\033[2J"

// --- 문자열 상수 (Phase 1-4) ---
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
enum QuestionType
{
    CarType_Q,
    Engine_Q,
    brakeSystem_Q,
    SteeringSystem_Q,
    Run_Test,
};

enum CarType
{
    SEDAN = 1,
    SUV,
    TRUCK
};

enum Engine
{
    GM = 1,
    TOYOTA,
    WIA,
    BROKEN  // Phase 1-2: 매직 넘버 4 제거
};

enum brakeSystem
{
    MANDO = 1,
    CONTINENTAL,
    BOSCH_B
};

enum SteeringSystem
{
    BOSCH_S = 1,
    MOBIS
};

// --- 전역 상태 ---
int stack[10];

// --- 전방 선언 ---
void selectCarType(int answer);
void selectEngine(int answer);
void selectBrakeSystem(int answer);
void selectSteeringSystem(int answer);
void runCar();
void runTest();
bool isSedanContinentalConflict();
bool isSuvToyotaConflict();
bool isTruckWiaConflict();
bool isTruckMandoConflict();
bool isBoschBrakeWithoutBoschSteer();
bool isValidCheck();
void delay(int ms);

// -----------------------------------------------
// 유틸리티
// -----------------------------------------------
void delay(int ms)
{
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
            for (int t = 0; t < ms; t++)
                sum++;
}

// -----------------------------------------------
// 부품 선택 함수 (Phase 1-1: 네이밍 통일)
// -----------------------------------------------
void selectCarType(int answer)
{
    stack[CarType_Q] = answer;
    if (answer == SEDAN)  printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_SEDAN);
    if (answer == SUV)    printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_SUV);
    if (answer == TRUCK)  printf("차량 타입으로 %s을 선택하셨습니다.\n", CAR_TRUCK);
}

void selectEngine(int answer)
{
    stack[Engine_Q] = answer;
    if (answer == GM)     printf("%s 엔진을 선택하셨습니다.\n", ENG_GM);
    if (answer == TOYOTA) printf("%s 엔진을 선택하셨습니다.\n", ENG_TOYOTA);
    if (answer == WIA)    printf("%s 엔진을 선택하셨습니다.\n", ENG_WIA);
}

void selectBrakeSystem(int answer)
{
    stack[brakeSystem_Q] = answer;
    if (answer == MANDO)       printf("%s 제동장치를 선택하셨습니다.\n", BRK_MANDO);
    if (answer == CONTINENTAL) printf("%s 제동장치를 선택하셨습니다.\n", BRK_CONTINENTAL);
    if (answer == BOSCH_B)     printf("%s 제동장치를 선택하셨습니다.\n", BRK_BOSCH);
}

void selectSteeringSystem(int answer)
{
    stack[SteeringSystem_Q] = answer;
    if (answer == BOSCH_S) printf("%s 조향장치를 선택하셨습니다.\n", STR_BOSCH);
    if (answer == MOBIS)   printf("%s 조향장치를 선택하셨습니다.\n", STR_MOBIS);
}

// -----------------------------------------------
// 검증 규칙 함수 (Phase 1-3: 개별 함수 분리)
// -----------------------------------------------
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

bool isValidCheck()
{
    if (isSedanContinentalConflict())    return false;
    if (isSuvToyotaConflict())           return false;
    if (isTruckWiaConflict())            return false;
    if (isTruckMandoConflict())          return false;
    if (isBoschBrakeWithoutBoschSteer()) return false;
    return true;
}

// -----------------------------------------------
// 실행 / 테스트 함수
// -----------------------------------------------
void runCar()
{
    if (!isValidCheck())
    {
        printf("자동차가 동작되지 않습니다\n");
        return;
    }
    if (stack[Engine_Q] == BROKEN)  // Phase 1-2: 매직 넘버 제거
    {
        printf("엔진이 고장나있습니다.\n");
        printf("자동차가 움직이지 않습니다.\n");
        return;
    }
    if (stack[CarType_Q] == SEDAN)       printf("Car Type : %s\n", CAR_SEDAN);
    if (stack[CarType_Q] == SUV)         printf("Car Type : %s\n", CAR_SUV);
    if (stack[CarType_Q] == TRUCK)       printf("Car Type : %s\n", CAR_TRUCK);
    if (stack[Engine_Q] == GM)           printf("Engine : %s\n", ENG_GM);
    if (stack[Engine_Q] == TOYOTA)       printf("Engine : %s\n", ENG_TOYOTA);
    if (stack[Engine_Q] == WIA)          printf("Engine : %s\n", ENG_WIA);
    if (stack[brakeSystem_Q] == MANDO)       printf("Brake System : %s\n", BRK_MANDO);
    if (stack[brakeSystem_Q] == CONTINENTAL) printf("Brake System : %s\n", BRK_CONTINENTAL);
    if (stack[brakeSystem_Q] == BOSCH_B)     printf("Brake System : %s\n", BRK_BOSCH);
    if (stack[SteeringSystem_Q] == BOSCH_S)  printf("SteeringSystem : %s\n", STR_BOSCH);
    if (stack[SteeringSystem_Q] == MOBIS)    printf("SteeringSystem : %s\n", STR_MOBIS);
    printf("자동차가 동작됩니다.\n");
}

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

// -----------------------------------------------
// main: Debug = 테스트 러너 / Release = UI 루프
// -----------------------------------------------
#ifdef _DEBUG

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

int main()
{
    char buf[100];
    int step = CarType_Q;

    while (1)
    {
        if (step == CarType_Q)
        {
            printf(CLEAR_SCREEN);
            printf("        ______________\n");
            printf("       /|            | \n");
            printf("  ____/_|_____________|____\n");
            printf(" |                      O  |\n");
            printf(" '-(@)----------------(@)--'\n");
            printf("===============================\n");
            printf("어떤 차량 타입을 선택할까요?\n");
            printf("1. %s\n", CAR_SEDAN);
            printf("2. %s\n", CAR_SUV);
            printf("3. %s\n", CAR_TRUCK);
        }
        else if (step == Engine_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 엔진을 탑재할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. %s\n", ENG_GM);
            printf("2. %s\n", ENG_TOYOTA);
            printf("3. %s\n", ENG_WIA);
            printf("4. 고장난 엔진\n");
        }
        else if (step == brakeSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 제동장치를 선택할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. %s\n", BRK_MANDO);
            printf("2. %s\n", BRK_CONTINENTAL);
            printf("3. %s\n", BRK_BOSCH);
        }
        else if (step == SteeringSystem_Q)
        {
            printf(CLEAR_SCREEN);
            printf("어떤 조향장치를 선택할까요?\n");
            printf("0. 뒤로가기\n");
            printf("1. %s\n", STR_BOSCH);
            printf("2. %s\n", STR_MOBIS);
        }
        else if (step == Run_Test)
        {
            printf(CLEAR_SCREEN);
            printf("멋진 차량이 완성되었습니다.\n");
            printf("어떤 동작을 할까요?\n");
            printf("0. 처음 화면으로 돌아가기\n");
            printf("1. RUN\n");
            printf("2. Test\n");
        }
        printf("===============================\n");
        printf("INPUT > ");
        fgets(buf, sizeof(buf), stdin);

        char *context = nullptr;
        strtok_s(buf, "\r", &context);
        strtok_s(buf, "\n", &context);

        if (!strcmp(buf, "exit"))
        {
            printf("바이바이\n");
            break;
        }

        char *checkNumber;
        int answer = strtol(buf, &checkNumber, 10);

        if (*checkNumber != '\0')
        {
            printf("ERROR :: 숫자만 입력 가능\n");
            delay(800);
            continue;
        }

        if (step == CarType_Q && !(answer >= 1 && answer <= 3))
        {
            printf("ERROR :: 차량 타입은 1 ~ 3 범위만 선택 가능\n");
            delay(800);
            continue;
        }
        if (step == Engine_Q && !(answer >= 0 && answer <= 4))
        {
            printf("ERROR :: 엔진은 1 ~ 4 범위만 선택 가능\n");
            delay(800);
            continue;
        }
        if (step == brakeSystem_Q && !(answer >= 0 && answer <= 3))
        {
            printf("ERROR :: 제동장치는 1 ~ 3 범위만 선택 가능\n");
            delay(800);
            continue;
        }
        if (step == SteeringSystem_Q && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: 조향장치는 1 ~ 2 범위만 선택 가능\n");
            delay(800);
            continue;
        }
        if (step == Run_Test && !(answer >= 0 && answer <= 2))
        {
            printf("ERROR :: Run 또는 Test 중 하나를 선택 필요\n");
            delay(800);
            continue;
        }

        if (answer == 0 && step == Run_Test)
        {
            step = CarType_Q;
            continue;
        }
        if (answer == 0 && step >= 1)
        {
            step -= 1;
            continue;
        }

        if (step == CarType_Q)
        {
            selectCarType(answer);
            delay(800);
            step = Engine_Q;
        }
        else if (step == Engine_Q)
        {
            selectEngine(answer);
            delay(800);
            step = brakeSystem_Q;
        }
        else if (step == brakeSystem_Q)
        {
            selectBrakeSystem(answer);
            delay(800);
            step = SteeringSystem_Q;
        }
        else if (step == SteeringSystem_Q)
        {
            selectSteeringSystem(answer);
            delay(800);
            step = Run_Test;
        }
        else if (step == Run_Test && answer == 1)
        {
            runCar();
            delay(2000);
        }
        else if (step == Run_Test && answer == 2)
        {
            printf("Test...\n");
            delay(1500);
            runTest();
            delay(2000);
        }
    }
    return 0;
}

#endif

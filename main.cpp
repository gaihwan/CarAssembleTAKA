// CarAssemble - main.cpp
// Phase 3: CarAssembler + CarValidator 사용. main()은 UI 루프만 담당.

#ifdef _DEBUG

#include "gmock/gmock.h"

int main()
{
    testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}

#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "car_config.h"
#include "CarValidator.h"
#include "CarAssembler.h"

#define CLEAR_SCREEN "\033[H\033[2J"

void delay(int ms)
{
    volatile int sum = 0;
    for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
            for (int t = 0; t < ms; t++)
                sum++;
}

int main()
{
    char buf[100];
    int step = CarType_Q;
    CarValidator validator;
    CarAssembler assembler(&validator);

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

        char* context = nullptr;
        strtok_s(buf, "\r", &context);
        strtok_s(buf, "\n", &context);

        if (!strcmp(buf, "exit"))
        {
            printf("바이바이\n");
            break;
        }

        char* checkNumber;
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
            assembler.reset();
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
            assembler.selectCarType(answer);
            delay(800);
            step = Engine_Q;
        }
        else if (step == Engine_Q)
        {
            assembler.selectEngine(answer);
            delay(800);
            step = brakeSystem_Q;
        }
        else if (step == brakeSystem_Q)
        {
            assembler.selectBrakeSystem(answer);
            delay(800);
            step = SteeringSystem_Q;
        }
        else if (step == SteeringSystem_Q)
        {
            assembler.selectSteeringSystem(answer);
            delay(800);
            step = Run_Test;
        }
        else if (step == Run_Test && answer == 1)
        {
            assembler.runCar();
            delay(2000);
        }
        else if (step == Run_Test && answer == 2)
        {
            printf("Test...\n");
            delay(1500);
            assembler.runTest();
            delay(2000);
        }
    }
    return 0;
}

#endif

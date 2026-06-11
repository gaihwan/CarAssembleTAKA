```cpp
// Phase 1 Unit Tests — test_phase1.cpp
// PLANS.md Phase 1-2, 1-3 검증
// 빌드: Debug 구성 (_DEBUG 정의)

#include <gtest/gtest.h>

// main.cpp 에 정의된 전역 심볼 참조
extern int stack[];

// QuestionType 인덱스 (main.cpp 와 값 동일)
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

// -----------------------------------------------
// [P1-T1] BROKEN enum 값 검증 (Phase 1-2)
// -----------------------------------------------
TEST(EnumTest, BrokenEngine_IsDefinedAsValue4)
{
    EXPECT_EQ(static_cast<int>(BROKEN), 4);
}

// -----------------------------------------------
// [P1-T2] 규칙 함수 개별 테스트 (Phase 1-3)
// -----------------------------------------------
TEST(RuleTest, SedanContinental_DetectsConflict)
{
    stack[CarType_Q]     = SEDAN;
    stack[brakeSystem_Q] = CONTINENTAL;
    EXPECT_TRUE(isSedanContinentalConflict());
}
TEST(RuleTest, SedanMando_NoConflict)
{
    stack[CarType_Q]     = SEDAN;
    stack[brakeSystem_Q] = MANDO;
    EXPECT_FALSE(isSedanContinentalConflict());
}
TEST(RuleTest, SuvToyota_DetectsConflict)
{
    stack[CarType_Q] = SUV;
    stack[Engine_Q]  = TOYOTA;
    EXPECT_TRUE(isSuvToyotaConflict());
}
TEST(RuleTest, SuvGM_NoConflict)
{
    stack[CarType_Q] = SUV;
    stack[Engine_Q]  = GM;
    EXPECT_FALSE(isSuvToyotaConflict());
}
TEST(RuleTest, TruckWia_DetectsConflict)
{
    stack[CarType_Q] = TRUCK;
    stack[Engine_Q]  = WIA;
    EXPECT_TRUE(isTruckWiaConflict());
}
TEST(RuleTest, TruckGM_NoConflict)
{
    stack[CarType_Q] = TRUCK;
    stack[Engine_Q]  = GM;
    EXPECT_FALSE(isTruckWiaConflict());
}
TEST(RuleTest, TruckMando_DetectsConflict)
{
    stack[CarType_Q]     = TRUCK;
    stack[brakeSystem_Q] = MANDO;
    EXPECT_TRUE(isTruckMandoConflict());
}
TEST(RuleTest, TruckBosch_NoConflict)
{
    stack[CarType_Q]     = TRUCK;
    stack[brakeSystem_Q] = BOSCH_B;
    EXPECT_FALSE(isTruckMandoConflict());
}
TEST(RuleTest, BoschBrakeWithMobis_DetectsConflict)
{
    stack[brakeSystem_Q]    = BOSCH_B;
    stack[SteeringSystem_Q] = MOBIS;
    EXPECT_TRUE(isBoschBrakeWithoutBoschSteer());
}
TEST(RuleTest, BoschBrakeWithBoschSteer_NoConflict)
{
    stack[brakeSystem_Q]    = BOSCH_B;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_FALSE(isBoschBrakeWithoutBoschSteer());
}

// -----------------------------------------------
// [P1-T3] isValidCheck() 종합 (Phase 1-3)
// -----------------------------------------------
TEST(ValidCheckTest, AllValid_ReturnsTrue)
{
    stack[CarType_Q]        = SEDAN;
    stack[Engine_Q]         = GM;
    stack[brakeSystem_Q]    = MANDO;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_TRUE(isValidCheck());
}
TEST(ValidCheckTest, Rule1_SedanContinental_ReturnsFalse)
{
    stack[CarType_Q]        = SEDAN;
    stack[Engine_Q]         = GM;
    stack[brakeSystem_Q]    = CONTINENTAL;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_FALSE(isValidCheck());
}
TEST(ValidCheckTest, Rule2_SuvToyota_ReturnsFalse)
{
    stack[CarType_Q]        = SUV;
    stack[Engine_Q]         = TOYOTA;
    stack[brakeSystem_Q]    = MANDO;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_FALSE(isValidCheck());
}
TEST(ValidCheckTest, Rule3_TruckWia_ReturnsFalse)
{
    stack[CarType_Q]        = TRUCK;
    stack[Engine_Q]         = WIA;
    stack[brakeSystem_Q]    = CONTINENTAL;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_FALSE(isValidCheck());
}
TEST(ValidCheckTest, Rule4_TruckMando_ReturnsFalse)
{
    stack[CarType_Q]        = TRUCK;
    stack[Engine_Q]         = GM;
    stack[brakeSystem_Q]    = MANDO;
    stack[SteeringSystem_Q] = BOSCH_S;
    EXPECT_FALSE(isValidCheck());
}
TEST(ValidCheckTest, Rule5_BoschBrakeWithMobis_ReturnsFalse)
{
    stack[CarType_Q]        = SEDAN;
    stack[Engine_Q]         = GM;
    stack[brakeSystem_Q]    = BOSCH_B;
    stack[SteeringSystem_Q] = MOBIS;
    EXPECT_FALSE(isValidCheck());
}
```

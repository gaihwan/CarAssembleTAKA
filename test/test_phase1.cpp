// Phase 1 Unit Tests — test_phase1.cpp
// Phase 2 이후 업데이트: extern stack[] → CarConfig API 사용

#include <gtest/gtest.h>
#include "../car_config.h"
#include "../validator.h"

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
    CarConfig c;
    c.carType = SEDAN;
    c.brake   = CONTINENTAL;
    EXPECT_TRUE(isSedanContinentalConflict(c));
}
TEST(RuleTest, SedanMando_NoConflict)
{
    CarConfig c;
    c.carType = SEDAN;
    c.brake   = MANDO;
    EXPECT_FALSE(isSedanContinentalConflict(c));
}
TEST(RuleTest, SuvToyota_DetectsConflict)
{
    CarConfig c;
    c.carType = SUV;
    c.engine  = TOYOTA;
    EXPECT_TRUE(isSuvToyotaConflict(c));
}
TEST(RuleTest, SuvGM_NoConflict)
{
    CarConfig c;
    c.carType = SUV;
    c.engine  = GM;
    EXPECT_FALSE(isSuvToyotaConflict(c));
}
TEST(RuleTest, TruckWia_DetectsConflict)
{
    CarConfig c;
    c.carType = TRUCK;
    c.engine  = WIA;
    EXPECT_TRUE(isTruckWiaConflict(c));
}
TEST(RuleTest, TruckGM_NoConflict)
{
    CarConfig c;
    c.carType = TRUCK;
    c.engine  = GM;
    EXPECT_FALSE(isTruckWiaConflict(c));
}
TEST(RuleTest, TruckMando_DetectsConflict)
{
    CarConfig c;
    c.carType = TRUCK;
    c.brake   = MANDO;
    EXPECT_TRUE(isTruckMandoConflict(c));
}
TEST(RuleTest, TruckBosch_NoConflict)
{
    CarConfig c;
    c.carType = TRUCK;
    c.brake   = BOSCH_B;
    EXPECT_FALSE(isTruckMandoConflict(c));
}
TEST(RuleTest, BoschBrakeWithMobis_DetectsConflict)
{
    CarConfig c;
    c.brake    = BOSCH_B;
    c.steering = MOBIS;
    EXPECT_TRUE(isBoschBrakeWithoutBoschSteer(c));
}
TEST(RuleTest, BoschBrakeWithBoschSteer_NoConflict)
{
    CarConfig c;
    c.brake    = BOSCH_B;
    c.steering = BOSCH_S;
    EXPECT_FALSE(isBoschBrakeWithoutBoschSteer(c));
}

// -----------------------------------------------
// [P1-T3] isValidCheck() 종합 (Phase 1-3)
// -----------------------------------------------
TEST(ValidCheckTest, AllValid_ReturnsTrue)
{
    CarConfig c{ SEDAN, GM, MANDO, BOSCH_S };
    EXPECT_TRUE(isValidCheck(c));
}
TEST(ValidCheckTest, Rule1_SedanContinental_ReturnsFalse)
{
    CarConfig c{ SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}
TEST(ValidCheckTest, Rule2_SuvToyota_ReturnsFalse)
{
    CarConfig c{ SUV, TOYOTA, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}
TEST(ValidCheckTest, Rule3_TruckWia_ReturnsFalse)
{
    CarConfig c{ TRUCK, WIA, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}
TEST(ValidCheckTest, Rule4_TruckMando_ReturnsFalse)
{
    CarConfig c{ TRUCK, GM, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}
TEST(ValidCheckTest, Rule5_BoschBrakeWithMobis_ReturnsFalse)
{
    CarConfig c{ SEDAN, GM, BOSCH_B, MOBIS };
    EXPECT_FALSE(isValidCheck(c));
}

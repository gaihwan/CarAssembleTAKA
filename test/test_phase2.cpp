// Phase 2 Unit Tests — test_phase2.cpp
// PLANS.md Phase 2-2 검증: CarConfig 구조체 + isValidCheck(CarConfig)

#include <gtest/gtest.h>
#include "../car_config.h"
#include "../validator.h"

// -----------------------------------------------
// [P2-T1] CarConfig 구조체 기본 초기화 (Phase 2-2)
// -----------------------------------------------
TEST(CarConfigTest, DefaultInitialization)
{
    CarConfig config{};
    EXPECT_EQ(config.carType,  static_cast<CarType>(0));
    EXPECT_EQ(config.engine,   static_cast<Engine>(0));
    EXPECT_EQ(config.brake,    static_cast<brakeSystem>(0));
    EXPECT_EQ(config.steering, static_cast<SteeringSystem>(0));
}

TEST(CarConfigTest, FieldAssignment)
{
    CarConfig config;
    config.carType  = SEDAN;
    config.engine   = GM;
    config.brake    = MANDO;
    config.steering = BOSCH_S;
    EXPECT_EQ(config.carType,  SEDAN);
    EXPECT_EQ(config.engine,   GM);
    EXPECT_EQ(config.brake,    MANDO);
    EXPECT_EQ(config.steering, BOSCH_S);
}

TEST(CarConfigTest, AggregateInitialization)
{
    CarConfig c{ TRUCK, WIA, BOSCH_B, MOBIS };
    EXPECT_EQ(c.carType,  TRUCK);
    EXPECT_EQ(c.engine,   WIA);
    EXPECT_EQ(c.brake,    BOSCH_B);
    EXPECT_EQ(c.steering, MOBIS);
}

// -----------------------------------------------
// [P2-T2] isValidCheck(CarConfig) — 규칙별 검증 (Phase 2-2)
// -----------------------------------------------
TEST(CarConfigValidationTest, Rule1_SedanContinental_ReturnsFalse)
{
    CarConfig c{ SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule2_SuvToyota_ReturnsFalse)
{
    CarConfig c{ SUV, TOYOTA, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule3_TruckWia_ReturnsFalse)
{
    CarConfig c{ TRUCK, WIA, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule4_TruckMando_ReturnsFalse)
{
    CarConfig c{ TRUCK, GM, MANDO, BOSCH_S };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, Rule5_BoschBrakeWithoutBoschSteer_ReturnsFalse)
{
    CarConfig c{ SEDAN, GM, BOSCH_B, MOBIS };
    EXPECT_FALSE(isValidCheck(c));
}

TEST(CarConfigValidationTest, AllRulesPass_ReturnsTrue)
{
    CarConfig c{ SEDAN, GM, MANDO, BOSCH_S };
    EXPECT_TRUE(isValidCheck(c));
}

TEST(CarConfigValidationTest, TruckValidCombination_ReturnsTrue)
{
    CarConfig c{ TRUCK, GM, CONTINENTAL, BOSCH_S };
    EXPECT_TRUE(isValidCheck(c));
}

TEST(CarConfigValidationTest, BoschBrakeWithBoschSteer_ReturnsTrue)
{
    CarConfig c{ SEDAN, GM, BOSCH_B, BOSCH_S };
    EXPECT_TRUE(isValidCheck(c));
}

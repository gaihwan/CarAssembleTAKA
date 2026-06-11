// Phase 3 Unit Tests — test_phase3.cpp
// PLANS.md Phase 3-2, 3-3 검증: CarValidator + MockValidator로 CarAssembler 독립 테스트

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../car_config.h"
#include "../IValidator.h"
#include "../CarValidator.h"
#include "../CarAssembler.h"

using ::testing::Return;
using ::testing::_;

// -----------------------------------------------
// [P3-T1] CarValidator 클래스 규칙별 테스트 (Phase 3-2)
// -----------------------------------------------
class CarValidatorTest : public ::testing::Test
{
protected:
    CarValidator validator;
};

TEST_F(CarValidatorTest, SedanContinental_IsInvalid)
{
    CarConfig c{ SEDAN, GM, CONTINENTAL, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, SuvToyota_IsInvalid)
{
    CarConfig c{ SUV, TOYOTA, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, TruckWia_IsInvalid)
{
    CarConfig c{ TRUCK, WIA, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, TruckMando_IsInvalid)
{
    CarConfig c{ TRUCK, GM, MANDO, BOSCH_S };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, BoschBrakeWithoutBoschSteer_IsInvalid)
{
    CarConfig c{ SEDAN, GM, BOSCH_B, MOBIS };
    EXPECT_FALSE(validator.isValid(c));
}

TEST_F(CarValidatorTest, ValidConfig_ReturnsTrue)
{
    CarConfig c{ TRUCK, GM, CONTINENTAL, BOSCH_S };
    EXPECT_TRUE(validator.isValid(c));
}

// -----------------------------------------------
// MockValidator 정의
// -----------------------------------------------
class MockValidator : public IValidator
{
public:
    MOCK_METHOD(bool, isValid, (const CarConfig&), (override));
};

// -----------------------------------------------
// [P3-T2] MockValidator로 CarAssembler 독립 테스트 (Phase 3-3)
// runCar(): 유효 시 "Car Type" 출력, 무효/고장 시 미출력 — ASCII 비교로 안정성 확보
// runTest(): 유효 시 "PASS", 무효 시 "FAIL" 출력 — ASCII 비교
// -----------------------------------------------

TEST(CarAssemblerTest, RunCar_ValidConfig_PrintsCarType)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));

    CarAssembler assembler(&mock);
    assembler.selectCarType(1);        // SEDAN
    assembler.selectEngine(1);         // GM
    assembler.selectBrakeSystem(1);    // MANDO
    assembler.selectSteeringSystem(1); // BOSCH_S

    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Car Type"), std::string::npos);
}

TEST(CarAssemblerTest, RunCar_InvalidConfig_DoesNotPrintCarType)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));

    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("Car Type"), std::string::npos);
}

TEST(CarAssemblerTest, RunCar_BrokenEngine_DoesNotPrintCarType)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));

    CarAssembler assembler(&mock);
    assembler.selectEngine(4);  // BROKEN

    testing::internal::CaptureStdout();
    assembler.runCar();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output.find("Car Type"), std::string::npos);
}

TEST(CarAssemblerTest, RunTest_ValidConfig_PrintsPass)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(true));

    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runTest();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("PASS"), std::string::npos);
}

TEST(CarAssemblerTest, RunTest_InvalidConfig_PrintsFail)
{
    MockValidator mock;
    EXPECT_CALL(mock, isValid(_)).WillOnce(Return(false));

    CarAssembler assembler(&mock);
    testing::internal::CaptureStdout();
    assembler.runTest();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("FAIL"), std::string::npos);
}

TEST(CarAssemblerTest, SelectCarType_StoresCorrectly)
{
    MockValidator mock;
    CarAssembler assembler(&mock);
    assembler.selectCarType(2);  // SUV
    EXPECT_EQ(assembler.getConfig().carType, SUV);
}

TEST(CarAssemblerTest, SelectEngine_StoresCorrectly)
{
    MockValidator mock;
    CarAssembler assembler(&mock);
    assembler.selectEngine(3);  // WIA
    EXPECT_EQ(assembler.getConfig().engine, WIA);
}

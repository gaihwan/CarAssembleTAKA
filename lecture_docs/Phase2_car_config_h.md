```cpp
#pragma once

// --- 문자열 상수 ---
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
    BROKEN
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

// --- Phase 2-2: 전역 stack[] 대체 구조체 ---
struct CarConfig
{
    CarType        carType  = static_cast<CarType>(0);
    Engine         engine   = static_cast<Engine>(0);
    brakeSystem    brake    = static_cast<brakeSystem>(0);
    SteeringSystem steering = static_cast<SteeringSystem>(0);
};
```

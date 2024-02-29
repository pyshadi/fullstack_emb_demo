#include "unity.h"
#include "LM75Wrapper.h"

// Mock function if needed
float LM75_ReadTemp_Mock() {
    return 25.0; // Mock temperature value
}

void test_LM75_ReadTemp(void) {
    float temp = LM75_ReadTemp_Mock(); // Replace with actual function if you can simulate the hardware interaction
    TEST_ASSERT_FLOAT_WITHIN(0.1, 25.0, temp);
}

// Main function to run the test
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_LM75_ReadTemp);
    return UNITY_END();
}

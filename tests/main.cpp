#include <gtest/gtest.h>
#include "utils/timer.h"

int main(int argc, char** argv) {
    // Disable Timer printing for tests
    butter::Timer::s_enableTiming = false;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

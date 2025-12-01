#include <gtest/gtest.h>
#include "utils/timer.h"

int main(int argc, char** argv) {
    // Disable Timer printing for tests
    euclide::Timer::ENABLE_TIMING = false;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

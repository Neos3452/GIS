#define BOOST_TEST_MODULE MemoryHooks
#include <boost/test/included/unit_test.hpp>

#include "MemoryHooks.h"

#include <cassert>
#include <memory>
#include <vector>

struct NewDeleteMeasurmentsRestart {
    NewDeleteMeasurmentsRestart() {
        gEnableHeapMemoryMeasurement = false;
        assert(gCurrentUsedHeapMemory == 0);
        gMaximumUsedHeapMemory = 0;
        gTotalAllocatedHeapMemory = 0;
        gTotalAllocatedFlippedOver = 0;
        gTotalFreedHeapMemory = 0;
        gTotalFreedFlippedOver = 0;
    }

    ~NewDeleteMeasurmentsRestart() {
        gEnableHeapMemoryMeasurement = false;
        assert(gCurrentUsedHeapMemory == 0);
        gMaximumUsedHeapMemory = 0;
        gTotalAllocatedHeapMemory = 0;
        gTotalAllocatedFlippedOver = 0;
        gTotalFreedHeapMemory = 0;
        gTotalFreedFlippedOver = 0;
    }
};

BOOST_FIXTURE_TEST_SUITE(CustomNewDelete, NewDeleteMeasurmentsRestart)

BOOST_AUTO_TEST_CASE(NotEnabling)
{
    BOOST_TEST(gEnableHeapMemoryMeasurement == false);
    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    auto *ptr = new int;

    BOOST_TEST(gEnableHeapMemoryMeasurement == false);
    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    delete ptr;
}

BOOST_AUTO_TEST_CASE(IntAllocation)
{
    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    auto *ptr = new int;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 4);
    BOOST_TEST(gMaximumUsedHeapMemory == 4);
    BOOST_TEST(gTotalAllocatedHeapMemory == 4);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    delete ptr;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 4);
    BOOST_TEST(gTotalAllocatedHeapMemory == 4);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 4);
    BOOST_TEST(gTotalFreedFlippedOver == 0);
}

BOOST_AUTO_TEST_CASE(ArrayAllocation)
{
    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    auto *ptr = new char[100];
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 100);
    BOOST_TEST(gMaximumUsedHeapMemory == 100);
    BOOST_TEST(gTotalAllocatedHeapMemory == 100);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    delete[] ptr;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 100);
    BOOST_TEST(gTotalAllocatedHeapMemory == 100);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 100);
    BOOST_TEST(gTotalFreedFlippedOver == 0);
}

BOOST_AUTO_TEST_CASE(MultipleAllocations)
{
    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    auto *ptr = new int;
    delete ptr;
    ptr = new int;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 4);
    BOOST_TEST(gMaximumUsedHeapMemory == 4);
    BOOST_TEST(gTotalAllocatedHeapMemory == 8);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 4);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    delete ptr;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 4);
    BOOST_TEST(gTotalAllocatedHeapMemory == 8);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 8);
    BOOST_TEST(gTotalFreedFlippedOver == 0);
}

BOOST_AUTO_TEST_CASE(MultipleAllocations2)
{
    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    auto *ptr = new int;
    auto *ptr2 = new int;
    delete ptr2;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 4);
    BOOST_TEST(gMaximumUsedHeapMemory == 8);
    BOOST_TEST(gTotalAllocatedHeapMemory == 8);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 4);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    delete ptr;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 8);
    BOOST_TEST(gTotalAllocatedHeapMemory == 8);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 8);
    BOOST_TEST(gTotalFreedFlippedOver == 0);
}

BOOST_AUTO_TEST_CASE(StdVectorAllocations)
{
    using IntVector = std::vector<int>;
    std::unique_ptr<IntVector> uPtr = nullptr;

    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedHeapMemory == 0);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    uPtr = std::make_unique<IntVector>();
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == sizeof(IntVector));
    BOOST_TEST(gMaximumUsedHeapMemory == sizeof(IntVector));
    BOOST_TEST(gTotalAllocatedHeapMemory == sizeof(IntVector));
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    uPtr->reserve(100);
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == sizeof(IntVector) + sizeof(int)*100);
    BOOST_TEST(gMaximumUsedHeapMemory == sizeof(IntVector) + sizeof(int)*100);
    BOOST_TEST(gTotalAllocatedHeapMemory == sizeof(IntVector) + sizeof(int)*100);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == 0);
    BOOST_TEST(gTotalFreedFlippedOver == 0);

    gEnableHeapMemoryMeasurement = true;
    uPtr = nullptr;
    gEnableHeapMemoryMeasurement = false;

    BOOST_TEST(gCurrentUsedHeapMemory == 0);
    BOOST_TEST(gMaximumUsedHeapMemory == sizeof(IntVector) + sizeof(int)*100);
    BOOST_TEST(gTotalAllocatedHeapMemory == sizeof(IntVector) + sizeof(int)*100);
    BOOST_TEST(gTotalAllocatedFlippedOver == 0);
    BOOST_TEST(gTotalFreedHeapMemory == sizeof(IntVector) + sizeof(int)*100);
    BOOST_TEST(gTotalFreedFlippedOver == 0);
}

BOOST_AUTO_TEST_SUITE_END()

#pragma once

#include <cstddef>
#include <cstdint>

/*
 * Just add MemoryHooks.cpp file into your compilation unit and then:
 *
 * To track stack size, first save current size using getCurrentStackSize().
 * Then add inside functions of your choice updateMaximumStackSize(). After
 * measurement is done substract gMaximumStackSize from your saved stacks size.
 * (assuming stack grows downwards).
 *
 * To track heap size set gEnableHeapMemoryMeasurement to true. When you are
 * done measuring set it back to false. Relevant trackers will contain all the
 * data.
 *
 * You can reset measured data between measurements by manually setting the
 * trackers or using resetHeapMemoryMeasurments() for heap measurements.
 *
 * Multithreaded not supported.
 *
 * Assumed is byte (ie. char) == 8 bits.
 */

namespace memory_measure {

extern bool gEnableHeapMemoryMeasurement;
extern size_t gCurrentUsedHeapMemory;
extern size_t gMaximumUsedHeapMemory;
extern size_t gTotalAllocatedHeapMemory;
extern size_t gTotalAllocatedFlippedOver;
extern size_t gTotalFreedHeapMemory;
extern size_t gTotalFreedFlippedOver;

void resetMemoryMeasurments();


extern uintptr_t gMaximumStackSize;

uintptr_t getCurrentStackSize();
void updateMaximumStackSize();

} // namespace memory_measure

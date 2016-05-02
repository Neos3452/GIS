#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <new>
#include <limits>
#include <unordered_map>

/*
 * Just add this file into your compilation unit and when you are ready to
 * start measuring set gEnableHeapMemoryMeasurement to true. When you are done
 * set it back to false.
 *
 * You can reset measured data between measurements by manually setting
 * trackers manually.
 *
 * Multithreaded not supported.
 *
 * TODO: a way to handle deallocations which happens during the measurement but
 *       allocation was not recorded. Possible solution is to enable hashmap
 *       to record everything, but track memory only for measurement ie. mark
 *       other as irrelevant. (One example of this is some unordered_map rehash)
 */

bool gEnableHeapMemoryMeasurement = false;
size_t gCurrentUsedHeapMemory = 0;
size_t gMaximumUsedHeapMemory = 0;
// below should obviously add to 0
size_t gTotalAllocatedHeapMemory = 0;
size_t gTotalAllocatedFlippedOver = 0;
size_t gTotalFreedHeapMemory = 0;
size_t gTotalFreedFlippedOver = 0;

static std::unordered_map<void *, size_t> gPtrToSizeMap;

void resetMemoryMeasurments()
{
    gPtrToSizeMap.clear();
    gCurrentUsedHeapMemory = 0;
    gMaximumUsedHeapMemory = 0;
    gTotalAllocatedHeapMemory = 0;
    gTotalAllocatedFlippedOver = 0;
    gTotalFreedHeapMemory = 0;
    gTotalFreedFlippedOver = 0;
}

void* operator new(std::size_t sz) throw(std::bad_alloc)
{
    // http://stackoverflow.com/questions/7194127/how-should-i-write-iso-c-standard-conformant-custom-new-and-delete-operators/
    using namespace std;                 
    if (sz == 0) {                     // handle 0-byte requests                     
        sz = 1;                        // by treating them as
    }                                  // 1-byte requests

    void *ptr = nullptr;
    while (true) {
        //attempt to allocate size bytes;
        if ((ptr = std::malloc(sz))) {
            break;
        }

        //allocation was unsuccessful; find out what the current new-handling function is (see below)
        new_handler globalHandler = set_new_handler(0);
        set_new_handler(globalHandler);

        if (globalHandler) {             //If new_hander is registered call it
             (*globalHandler)();
        } else {
             throw std::bad_alloc();     //No handler is registered throw an exception
        }
    }

    // memory is allocated
    if (gEnableHeapMemoryMeasurement) {
        // we don't want unordered_map to be measured
        gEnableHeapMemoryMeasurement = false;
        {
            const auto inserted = gPtrToSizeMap.emplace(ptr, sz).second;
            assert(inserted);
        }
        gEnableHeapMemoryMeasurement = true;

        gCurrentUsedHeapMemory += sz;
        gMaximumUsedHeapMemory = std::max(gMaximumUsedHeapMemory, gCurrentUsedHeapMemory);
        if (std::numeric_limits<size_t>::max() - sz >= gTotalAllocatedHeapMemory) {
            gTotalAllocatedHeapMemory += sz;
        } else {
            ++gTotalAllocatedFlippedOver;
            gTotalAllocatedHeapMemory = std::numeric_limits<size_t>::max() - gTotalAllocatedHeapMemory;
        }
    }

    return ptr;
}

void operator delete(void *ptr) noexcept
{
    if (gEnableHeapMemoryMeasurement) {
        size_t sz = 0;
        // we don't want unordered_map to be measured
        gEnableHeapMemoryMeasurement = false;
        {
            const auto memIt = gPtrToSizeMap.find(ptr);
            // double free or free on garbage
            assert(memIt != gPtrToSizeMap.cend());
            sz = memIt->second;
            assert(sz > 0);
            gPtrToSizeMap.erase(memIt);
        }
        gEnableHeapMemoryMeasurement = true;

        // this indicates bad set up of the measurement
        assert(sz <= gCurrentUsedHeapMemory);
        gCurrentUsedHeapMemory -= sz;
        if (std::numeric_limits<size_t>::max() - sz >= gTotalFreedHeapMemory) {
            gTotalFreedHeapMemory += sz;
        } else {
            ++gTotalFreedFlippedOver;
            gTotalFreedHeapMemory = std::numeric_limits<size_t>::max() - gTotalFreedHeapMemory;
        }
    }
    std::free(ptr);
}

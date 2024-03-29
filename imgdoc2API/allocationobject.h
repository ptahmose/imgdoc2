#pragma once

#include <cstdint>

#pragma pack(push, 4)
struct AllocationObject
{
    void* pointer_to_memory;
    std::intptr_t handle;
};
#pragma pack(pop)

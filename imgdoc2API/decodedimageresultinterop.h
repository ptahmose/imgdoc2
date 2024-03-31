#pragma once

#include "allocationobject.h"

#pragma pack(push, 4)
struct DecodeImageResultInterop
{
    std::uint32_t stride;
    AllocationObject bitmap;
};
#pragma pack(pop)

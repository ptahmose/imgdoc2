#pragma once

#pragma pack(push, 4)
struct BitmapInfoInterop
{
    std::uint8_t pixelType;
    std::uint32_t pixelWidth;
    std::uint32_t pixelHeight;
};
#pragma pack(pop)

// SPDX-FileCopyrightText: 2024 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

namespace ImgDoc2Net.Interfaces
{
    using System;

    public interface IDecoding
    {
        BitmapData Decode(Span<byte> compressedData, DataType dataType, PixelType pixelType, int width, int height, int stride = -1);
    }
}

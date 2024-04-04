// SPDX-FileCopyrightText: 2024 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

namespace ImgDoc2Net.Interfaces
{
    using System;

    public struct BitmapData
    {
        public int Width { get; set; }

        public int Height { get; set; }

        public int Stride { get; set; }

        public PixelType PixelType { get; set; }

        public Memory<byte> Data { get; set; }
    }
}

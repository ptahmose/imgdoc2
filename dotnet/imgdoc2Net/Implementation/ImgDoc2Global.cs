// SPDX-FileCopyrightText: 2024 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

using System;

namespace ImgDoc2Net.Implementation
{
    using ImgDoc2Net.Interfaces;
    using ImgDoc2Net.Interop;

    public static class ImgDoc2Global
    {
        private static Lazy<IDecoding> decoder = new Lazy<IDecoding>(() => new Decoding());

        public static ImgDoc2VersionInfo GetVersionInfo()
        {
            ImgDoc2NativeLibraryVersionInfo nativeLibraryVersion = ImgDoc2ApiInterop.Instance.GetNativeLibraryVersionInfo();
            var versionInfo = new ImgDoc2VersionInfo();
            return new ImgDoc2VersionInfo
            {
                NativeLibraryVersion = nativeLibraryVersion,
                ManagedImgDoc2LibraryVersionInfo = "1.2.3",
            };
        }

        public static IDecoding GetDecoder()
        {
            return ImgDoc2Global.decoder.Value;
        }
    }
}
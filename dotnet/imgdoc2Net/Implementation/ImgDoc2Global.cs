// SPDX-FileCopyrightText: 2024 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

namespace ImgDoc2Net.Implementation
{
    using System;
    using ImgDoc2Net.Interfaces;
    using ImgDoc2Net.Interop;

    /// <summary> 
    /// Support functions and other context-free functions are provided by this class.
    /// </summary>
    public static class ImgDoc2Global
    {
        private static readonly Lazy<IDecoding> Decoder = new Lazy<IDecoding>(() => new Decoding());

        /// <summary> Gets version information.</summary>
        /// <returns> The version information.</returns>
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

        /// <summary> Gets a decoder object.</summary>
        /// <returns> The decoder object.</returns>
        public static IDecoding GetDecoder()
        {
            return ImgDoc2Global.Decoder.Value;
        }
    }
}
// SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

namespace ImgDoc2Net.Implementation
{
    /// <brief> 
    /// Version information about the native imgdoc2 library.
    /// </brief>
    public struct ImgDoc2NativeLibraryVersionInfo
    {
        public int Major { get; set; }
        public int Minor { get; set; }
        public int Patch { get; set; }
        public string CompilerIdentification { get; set; }
        public string BuildType { get; set; }
        public string RepositoryUrl { get; set; }
        public string RepositoryBranch { get; set; }
        public string RepositoryTag { get; set; }
    }

    public class ImgDoc2VersionInfo
    {
        public ImgDoc2NativeLibraryVersionInfo NativeLibraryVersion { get; set; }

        public string NativeImgDoc2LibraryVersionInfo { get; set; }

        public string NativeImgDoc2BuildInformation { get; set; }

        public string ManagedImgDoc2LibraryVersionInfo { get; set; }
    }
}

// SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

namespace ImgDoc2Net.Implementation
{
    public struct ImgDoc2NativeLibraryVersionInfo
    {
        public int Major;
        public int Minor;
        public int Patch;
        public string CompilerIdentification;
        public string BuildType;
        public string RepositoryUrl;
        public string RepositoryBranch;
        public string RepositoryTag;
    }

    public class ImgDoc2VersionInfo
    {
        public ImgDoc2NativeLibraryVersionInfo NativeLibraryVersion { get; set; }

        public string NativeImgDoc2LibraryVersionInfo { get; set; }

        public string NativeImgDoc2BuildInformation { get; set; }

        public string ManagedImgDoc2LibraryVersionInfo { get; set; }
    }
}

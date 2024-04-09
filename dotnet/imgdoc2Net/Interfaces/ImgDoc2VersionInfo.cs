// SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

namespace ImgDoc2Net.Interfaces
{
    /// <brief> 
    /// Version information about the native imgdoc2 library.
    /// </brief>
    public struct ImgDoc2NativeLibraryVersionInfo
    {
        /// <summary> Gets or sets the major version.</summary>
        /// <value> The major version.</value>
        public int Major { get; set; }

        /// <summary> Gets or sets the minor version.</summary>
        /// <value> The minor version.</value>
        public int Minor { get; set; }

        /// <summary> Gets or sets the patch version.</summary>
        /// <value> The patch version.</value>
        public int Patch { get; set; }

        /// <summary> Gets or sets the compiler identification - an informative string identifying the compiler.</summary>
        /// <value> The compiler identification.</value>
        public string CompilerIdentification { get; set; }

        /// <summary> Gets or sets the type of the build - an information string identifying the build type, e.g. "Debug" or "Release".</summary>
        /// <value> The build type.</value>
        public string BuildType { get; set; }

        /// <summary> Gets or sets URL of the repository.</summary>
        /// <value> The repository URL.</value>
        public string RepositoryUrl { get; set; }

        /// <summary> Gets or sets the repository branch.</summary>
        /// <value> The repository branch.</value>
        public string RepositoryBranch { get; set; }

        /// <summary> Gets or sets the repository tag.</summary>
        /// <value> The repository tag.</value>
        public string RepositoryTag { get; set; }
    }

    /// <brief> 
    /// Version information about the managed imgdoc2 interop library.
    /// </brief>
    public struct ImgDoc2ManagedLibraryVersionInfo
    {
        /// <summary> Gets or sets the major version.</summary>
        /// <value> The major version.</value>
        public int Major { get; set; }

        /// <summary> Gets or sets the minor version.</summary>
        /// <value> The minor version.</value>
        public int Minor { get; set; }

        /// <summary> Gets or sets the patch version.</summary>
        /// <value> The patch version.</value>
        public int Patch { get; set; }

        /// <summary> Gets or sets the reversion - this is always zero.</summary>
        /// <value> The reversion - always 0.</value>
        public int Revision { get; set; }

        /// <summary> Gets or sets the type of the build - an information string identifying the build type, e.g. "Debug" or "Release".</summary>
        /// <value> The build type.</value>
        public string BuildType { get; set; }
    }

    /// <summary> Version information about the ImgDoc2Net library.</summary>
    public class ImgDoc2VersionInfo
    {
        public ImgDoc2NativeLibraryVersionInfo NativeLibraryVersion { get; set; }

        public ImgDoc2ManagedLibraryVersionInfo ManagedImgDoc2LibraryVersionInfo { get; set; }
    }
}

// SPDX-FileCopyrightText: 2023 Carl Zeiss Microscopy GmbH
//
// SPDX-License-Identifier: MIT

namespace ImgDoc2Net.Interop
{
    using System;
    using System.Runtime.InteropServices;
    using System.Text;

    /// <summary> A bunch of utilities.</summary>
    internal static class Utilities
    {
        /// <summary> Gets a value indicating if the current platform is Linux.</summary>
        ///
        /// <returns> True if running on a Linux platform; false otherwise.</returns>
        public static bool IsLinux()
        {
            // http://stackoverflow.com/a/5117005/358336
            int p = (int)Environment.OSVersion.Platform;
            return (p == 4) || (p == 6) || (p == 128);
        }

        public static string ConvertFromUtf8IntPtr(IntPtr utf8Pointer, int length)
        {
            if (utf8Pointer == IntPtr.Zero)
                throw new ArgumentNullException(nameof(utf8Pointer), "Pointer is null.");

            // Copy data from unmanaged memory to a managed byte array
            byte[] buffer = new byte[length];
            Marshal.Copy(utf8Pointer, buffer, 0, length);

            // Convert the byte array to a string
            return Encoding.UTF8.GetString(buffer);
        }

        // When the length is unknown
        public static string ConvertFromUtf8IntPtrUnknownLength(IntPtr utf8Pointer)
        {
            // with .NETCore 2.1 we could use the following code instead:
            //  return Marshal.PtrToStringUTF8(utf8Pointer);
            if (utf8Pointer == IntPtr.Zero)
            {
                throw new ArgumentNullException(nameof(utf8Pointer), "Pointer is null.");
            }

            // Determine the length of the string
            int length = 0;
            while (Marshal.ReadByte(utf8Pointer, length) != 0)
            {
                length++;
            }

            return ConvertFromUtf8IntPtr(utf8Pointer, length);
        }
    }
}

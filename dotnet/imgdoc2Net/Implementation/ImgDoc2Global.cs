using ImgDoc2Net.Interop;

namespace ImgDoc2Net.Implementation
{
    public static class ImgDoc2Global
    {
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
    }
}
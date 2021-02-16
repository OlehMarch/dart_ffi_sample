// h file start
extern "C" std::string __stdcall GetNtDllVersion();

extern "C" void __declspec(dllexport) __stdcall hello_world();
// h file end

// cpp file start
#include <string>
#include <stdio.h>

#include <windows.h>

#pragma comment(lib, "Version.lib")

extern "C" std::string __stdcall GetNtDllVersion()
{
    std::string versionString;
    DWORD someHandle;
    wchar_t systemFolderPath[_MAX_PATH + 1];
    UINT systemFolderPathSize = GetSystemDirectoryW(systemFolderPath, _MAX_PATH);
    if (systemFolderPathSize > 0)
    {
        std::wstring ntDllPath(systemFolderPath, systemFolderPathSize);
        ntDllPath += L"\\ntoskrnl.exe";
        DWORD versionSize = GetFileVersionInfoSizeW(ntDllPath.c_str(), &someHandle);
        if (versionSize > 0)
        {
            LPVOID dataPtr = malloc(versionSize);
            if (dataPtr != NULL)
            {
                if (GetFileVersionInfoW(ntDllPath.c_str(), 0, versionSize, dataPtr))
                {
                    UINT length;
                    LPVOID outputPtr;
                    if (VerQueryValueW(dataPtr, L"\\", &outputPtr, &length))
                    {
                        VS_FIXEDFILEINFO *versionStructPtr = (VS_FIXEDFILEINFO *)outputPtr;
                        if (versionStructPtr->dwSignature == 0xFEEF04BD)
                        {
                            versionString = std::to_string(HIWORD(versionStructPtr->dwProductVersionMS));
                            versionString += ".";
                            versionString += std::to_string(LOWORD(versionStructPtr->dwProductVersionMS));
                            versionString += ".";
                            versionString += std::to_string(HIWORD(versionStructPtr->dwProductVersionLS));
                            versionString += ".";
                            versionString += std::to_string(LOWORD(versionStructPtr->dwProductVersionLS));
                        }
                    }
                }
                free(dataPtr);
            }
        }
    }
    return versionString;
}
extern "C" void __declspec(dllexport) __stdcall hello_world()
{
    auto res = GetNtDllVersion();
    printf("version: %s\n", res);
}
// cpp file end

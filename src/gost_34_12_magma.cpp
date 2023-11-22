#if defined(_WIN32)
#include <Windows.h>
#endif

#include <stdint.h>

#if defined(_WIN32)
//  Microsoft
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__linux)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import / export semantics.
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    uint64_t _GOST_encr(uint8_t *byPKey, uint32_t *dwA1, uint32_t *dwA0);
    uint64_t _GOST_decr(uint8_t *byPKey, uint32_t *dwA1, uint32_t *dwA0);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" EXPORT uint64_t encrypt(uint8_t *byPKey, uint64_t data_block)
{
#else
EXPORT uint64_t encrypt(uint8_t *byPKey, uint64_t data_block)
{
#endif
    union block
    {
        uint64_t data64;
        uint32_t data32[2];
    };
    block _block;
    _block.data64 = data_block;
    uint32_t dwA1 = _block.data32[1];
    uint32_t dwA0 = _block.data32[0];
    return _GOST_encr(byPKey, &dwA1, &dwA0);
}

#ifdef __cplusplus
extern "C" EXPORT uint64_t decrypt(uint8_t *byPKey, uint64_t data_block)
{
#else
EXPORT uint64_t decrypt(uint8_t *byPKey, uint64_t data_block)
{
#endif
    union block
    {
        uint64_t data64;
        uint32_t data32[2];
    };
    block _block;
    _block.data64 = data_block;
    uint32_t dwA1 = _block.data32[1];
    uint32_t dwA0 = _block.data32[0];
    return _GOST_decr(byPKey, &dwA1, &dwA0);
}

#if defined(_WIN32)
EXPORT BOOL WINAPI DllMain(
    HINSTANCE hinstDLL, // handle to DLL module
    DWORD fdwReason,    // reason for calling function
    LPVOID lpvReserved) // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:

        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.
        break;
    }
    return TRUE; // Successful DLL_PROCESS_ATTACH.
}
#endif
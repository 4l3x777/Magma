#pragma once

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
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#ifdef __cplusplus  
extern "C" {               
#endif

EXPORT uint64_t encrypt(uint8_t* byPKey, uint64_t data_block);

EXPORT uint64_t decrypt(uint8_t* byPKey, uint64_t data_block);

#ifdef __cplusplus  
}  
#endif
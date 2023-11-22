#pragma once

#include <stdint.h>

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
extern "C" uint64_t encrypt(uint8_t *byPKey, uint64_t data_block)
{
#else
uint64_t encrypt(uint8_t *byPKey, uint64_t data_block)
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
extern "C" uint64_t decrypt(uint8_t *byPKey, uint64_t data_block)
{
#else
uint64_t decrypt(uint8_t *byPKey, uint64_t data_block)
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
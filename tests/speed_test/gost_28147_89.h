#include <memory.h>
#include <stdint.h>
#include <utility>

extern "C"
{

	using substitution_t = uint8_t[128];

	substitution_t byArrPi = {
		0xc, 0x4, 0x6, 0x2, 0xa, 0x5, 0xb, 0x9, 0xe, 0x8, 0xd, 0x7, 0x0, 0x3, 0xf, 0x1,
		0x6, 0x8, 0x2, 0x3, 0x9, 0xa, 0x5, 0xc, 0x1, 0xe, 0x4, 0x7, 0xb, 0xd, 0x0, 0xf,
		0xb, 0x3, 0x5, 0x8, 0x2, 0xf, 0xa, 0xd, 0xe, 0x1, 0x7, 0x4, 0xc, 0x9, 0x6, 0x0,
		0xc, 0x8, 0x2, 0x1, 0xd, 0x4, 0xf, 0x6, 0x7, 0x0, 0xa, 0x5, 0x3, 0xe, 0x9, 0xb,
		0x7, 0xf, 0x5, 0xa, 0x8, 0x1, 0x6, 0xd, 0x0, 0x9, 0x3, 0xe, 0xb, 0x4, 0x2, 0xc,
		0x5, 0xd, 0xf, 0x6, 0x9, 0x2, 0xc, 0xa, 0xb, 0x7, 0x8, 0x1, 0x4, 0x3, 0xe, 0x0,
		0x8, 0xe, 0x2, 0x5, 0x6, 0x9, 0x1, 0xc, 0xf, 0x4, 0xb, 0x0, 0xd, 0xa, 0x3, 0x7,
		0x1, 0x7, 0xe, 0xd, 0x0, 0x5, 0x8, 0x3, 0x4, 0xf, 0xa, 0x6, 0x9, 0xc, 0xb, 0x2};

	void magma_round(uint32_t dwRoundkey, uint32_t &dwA1, uint32_t dwA0)
	{
		uint32_t g = dwA0 + dwRoundkey;

		uint32_t dwT =
			((byArrPi[0 + ((g & 0x0000000f) >> 0)]) << 0) | ((byArrPi[16 + ((g & 0x000000f0) >> 4)]) << 4) | ((byArrPi[32 + ((g & 0x00000f00) >> 8)]) << 8) | ((byArrPi[48 + ((g & 0x0000f000) >> 12)]) << 12) | ((byArrPi[64 + ((g & 0x000f0000) >> 16)]) << 16) | ((byArrPi[80 + ((g & 0x00f00000) >> 20)]) << 20) | ((byArrPi[96 + ((g & 0x0f000000) >> 24)]) << 24) | ((byArrPi[112 + ((g & 0xf0000000) >> 28)]) << 28);

		dwA1 ^= ((dwT << 11) | (dwT >> 21));
	}

	uint32_t *create_round_keys(uint8_t *pKey)
	{
		/* view of RoundKey in format of one byte SubKey */
		union key_formated
		{
			uint32_t dwRoundKey;
			uint8_t byArrSubKey[4];
		};
		key_formated format_Rkey;

		uint32_t *pRoundKeys = new uint32_t[32];

		for (uint8_t i = 0; i < 8; i++)
		{
			for (uint8_t j = 0; j < 4; j++)
			{
				format_Rkey.byArrSubKey[j] = pKey[4 * i + j];
			}
			pRoundKeys[7 - i] = format_Rkey.dwRoundKey;
		}

		for (uint8_t i = 8; i < 24; i++)
		{
			pRoundKeys[i] = pRoundKeys[i % 8];
		}
		for (uint8_t i = 24; i < 32; i++)
		{
			pRoundKeys[i] = pRoundKeys[7 - (i % 8)];
		}
		return pRoundKeys;
	}

	/* Encrypt memory block */
	uint64_t magma_encrypt_block(uint32_t *pRoundKeys, uint64_t qwBlock)
	{
		uint32_t dwA0 = qwBlock & 0xffffffff;
		uint32_t dwA1 = qwBlock >> 32;

		for (uint8_t i = 0; i < 32; i++)
		{
			magma_round(pRoundKeys[i], dwA1, dwA0);
			std::swap(dwA1, dwA0);
		}

		uint64_t qwResult = dwA0;
		qwResult = qwResult << 32;
		return qwResult ^ dwA1;
	}

	/* Decrypt memory block */
	uint64_t magma_decrypt_block(uint32_t *pRoundKeys, uint64_t qwBlock)
	{
		uint32_t dwA0 = qwBlock & 0xffffffff;
		uint32_t dwA1 = qwBlock >> 32;

		for (uint8_t i = 32; i > 0; i--)
		{
			magma_round(pRoundKeys[i - 1], dwA1, dwA0);
			std::swap(dwA1, dwA0);
		}

		uint64_t qwResult = dwA0;
		qwResult = qwResult << 32;
		return qwResult ^ dwA1;
	};
}
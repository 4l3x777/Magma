#include <gtest/gtest.h>
#include <iostream>

extern "C"
{
	uint64_t _GOST_encr(uint8_t *byPKey, uint32_t *dwA1, uint32_t *dwA0);
	uint64_t _GOST_decr(uint8_t *byPKey, uint32_t *dwA1, uint32_t *dwA0);
	uint32_t _t_proc(uint32_t dwA);
	uint32_t _g_proc(uint32_t dwK, uint32_t dwA);
	void _G1_proc(uint32_t dwK, uint32_t *dwA1, uint32_t *dwA0);
	uint64_t _G2_proc(uint32_t dwK, uint32_t *dwA1, uint32_t *dwA0);
	uint32_t _RKey_proc(uint8_t *byPKey, uint8_t byRNum);
}

class TestMagma : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}
};

TEST_F(TestMagma, test_t_proc)
{
	EXPECT_TRUE(_t_proc(_t_proc(_t_proc(_t_proc(0xfdb97531)))) == 0x68695433);
}

TEST_F(TestMagma, test_g_proc)
{
	uint32_t test_Key = 0x87654321;
	uint32_t test_A = 0xfedcba98;
	uint32_t test_A2 = 0xfdcbc20c;
	uint32_t test_A3 = 0x7e791a4b;
	EXPECT_TRUE(_g_proc(_g_proc(_g_proc(_g_proc(test_Key, test_A), test_Key), test_A2), test_A3) == 0x9791c849);
}

TEST_F(TestMagma, test_G1_proc)
{
	uint32_t test_A1 = 0xfedcba98;
	uint32_t test_A0 = 0x76543210;
	uint32_t test_K1 = 0xffeeddcc;
	_G1_proc(test_K1, &test_A1, &test_A0);
	EXPECT_TRUE(test_A1 == 0x76543210 && test_A0 == 0x28da3b14);
}

TEST_F(TestMagma, test_G2_proc)
{
	uint32_t test_B1 = 0x28da3b14;
	uint32_t test_B0 = 0x76543210;
	uint32_t test_K1 = 0xffeeddcc;
	EXPECT_TRUE(_G2_proc(test_K1, &test_B1, &test_B0) == 0xfedcba9876543210);
}

TEST_F(TestMagma, test_RKey_proc)
{
	uint8_t key[32] = {0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
	uint8_t byRNum = 20;
	EXPECT_TRUE(_RKey_proc(key, byRNum) == 0x33221100);
	byRNum = 24;
	EXPECT_TRUE(_RKey_proc(key, byRNum) == 0xfcfdfeff);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
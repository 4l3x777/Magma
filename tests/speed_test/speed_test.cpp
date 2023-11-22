#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <vector>

#include "gost_28147_89.h"
#include "../../include/gost_34_12_magma_static.h"

using namespace	std;

class TestSpeedTest : public ::testing::Test 
{
protected:
	void SetUp() {}
	void TearDown() {}
};

TEST_F(TestSpeedTest, test_speed)
{
	// speed test ASM ans C++/C realizations
	uint8_t key[32] = { 0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 0xf6, 0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
	uint64_t testA1A0 = 0xfedcba9876543210;

    // emulate number of encryption blocks
	uint32_t NumBlocks = 1310720; // 10Mbyte data emulate
	chrono::time_point<chrono::high_resolution_clock> start, stop;

	start = chrono::high_resolution_clock::now();
    uint32_t* round_keys = create_round_keys(key);
	for (uint32_t i = 0; i < NumBlocks; i++) magma_encrypt_block(round_keys, testA1A0);
	stop = chrono::high_resolution_clock::now();
	cout << endl << "C++/C 10Mbyte encryption time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " milliseconds" << endl;

	start = chrono::high_resolution_clock::now();
	for (uint64_t i = 0; i < NumBlocks; i++) encrypt(key, testA1A0);
	stop = chrono::high_resolution_clock::now();
	cout << endl << "ASM 10Mbyte encryption time: " << chrono::duration_cast<chrono::milliseconds>(stop - start).count() << " milliseconds" << endl;
	
    EXPECT_TRUE(true);
}
#include "RandomNumberGenerator.h"

static uint32_t nLehmer = 0;
int LehmerInt() {
	nLehmer += 0xe120fc15;
	uint64_t temp;
	temp = (uint64_t)nLehmer * 0x4a39b70d;
	uint32_t m1 = (temp >> 32) ^ temp;
	temp = (uint64_t)m1 * 0x12fad5c9;
	uint32_t m2 = (temp >> 32) ^ temp;
	return m2;
}

void setLehmerSeed(int seed) {
	nLehmer = seed;
}
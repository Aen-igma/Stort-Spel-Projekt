#include "PCH/PCH.h"

#include "RandomNumberGenerator.h"

static uint64_t nLehmer = 0;
static uint64_t lehmerConstSeed = 0;

const uint64_t LehmerInt() {
	nLehmer += 0xe120fc15;
	uint64_t temp;
	temp = (uint64_t)nLehmer * 0x4a39b70d;
	uint64_t m1 = (temp >> 32) ^ temp;
	temp = (uint64_t)m1 * 0x12fad5c9;
	uint64_t m2 = (temp >> 32) ^ temp;
	return m2;
}

void SetLehmerSeed(const int seed) {
	nLehmer = seed + lehmerConstSeed;
}

void SetLehmerConstSeed(const int seed)
{
	lehmerConstSeed = seed;
}

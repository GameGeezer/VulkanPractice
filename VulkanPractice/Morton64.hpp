#pragma once

#include <stdint.h>

typedef uint64_t MortonCode64;

class Morton64
{
	static MortonCode64
	encode(uint32_t x, uint32_t y, uint32_t z)
	{
		uint64_t morton = 0;
		morton |= splitBy3(x) | splitBy3(y) << 1 | splitBy3(z) << 2;
		return morton;
	}

	static uint64_t
	highestOrderBitDifferent(MortonCode64 first, MortonCode64 second)
	{
		return highestOrderBit(first ^ second);
	}

private:

	static uint64_t
	splitBy3(uint32_t value)
	{
		uint64_t x = value & 0x1fffff;

		x = (x | x << 32) & 0x1f00000000ffff;
		x = (x | x << 16) & 0x1f0000ff0000ff;
		x = (x | x << 8) & 0x100f00f00f00f00f;
		x = (x | x << 4) & 0x10c30c30c30c30c3;
		x = (x | x << 2) & 0x1249249249249249;

		return x;
	}

	static uint64_t
	highestOrderBit(uint64_t n)
	{
		n |= (n >> 1);
		n |= (n >> 2);
		n |= (n >> 4);
		n |= (n >> 8);
		n |= (n >> 16);
		n |= (n >> 32);

		return n - (n >> 1);
	}
};
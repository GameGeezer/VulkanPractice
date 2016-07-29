#pragma once

#include <stdint.h>

typedef MortonCode30;

// A morton code where 10 bits are allocated to x, y, z each.
class Morton30
{
public:

	static MortonCode30
	encode(uint32_t x, uint32_t y, uint32_t z)
	{
		return splitBy3(x) | (splitBy3(y) << 1) | (splitBy3(z) << 2);
	}

	static void
	decode(MortonCode30 morton, uint32_t& x, uint32_t& y, uint32_t& z)
	{
		x = compactBy3(morton);
		y = compactBy3(morton >> 1);
		z = compactBy3(morton >> 2);
	}

	static uint32_t
	highestOrderBitDifferent(MortonCode30 first, MortonCode30 second)
	{
		return highestOrderBit(first ^ second);
	}

private:

	static uint32_t
	splitBy3(uint32_t value)
	{
		value &= 0x000003ff;
		value |= (value << 16);
		value &= 0x030000ff;
		value |= (value << 8);
		value &= 0x0300f00f;
		value |= (value << 4);
		value &= 0x030c30c3;
		value |= (value << 2);
		value &= 0x09249249;

		return value;
	}

	static uint32_t
	compactBy3(uint32_t value)
	{
		value &= 0x09249249;
		value |= (value >> 2);
		value &= 0x030c30c3;
		value |= (value >> 4);
		value &= 0x0300f00f;
		value |= (value >> 8);
		value &= 0x030000ff;
		value |= (value >> 16);
		value &= 0x000003ff;

		return value;
	}

	static uint32_t
	highestOrderBit(uint32_t n)
	{
		n |= (n >> 1);
		n |= (n >> 2);
		n |= (n >> 4);
		n |= (n >> 8);
		n |= (n >> 16);

		return n - (n >> 1);
	}
};
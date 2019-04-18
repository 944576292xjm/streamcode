#pragma once

#include <stdint.h>
namespace base {


class BinaryReader
{

public:
	BinaryReader(const unsigned char* data, size_t size, bool bigEndian = true);
	~BinaryReader();

	unsigned char  Read8();
	unsigned short Read16();
	unsigned int Read24();
	unsigned int Read32();
	uint64_t Read64();
	unsigned int  Read(unsigned char* value, unsigned int size);
	unsigned int Available();
	unsigned int Next(unsigned int count = 1);
	const unsigned char*  Current();
	static unsigned short Flip16(unsigned short value) { return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00); }
	static unsigned int Flip24(unsigned int value) { return ((value >> 16) & 0x000000FF) | (value & 0x0000FF00) | ((value << 16) & 0x00FF0000); }
	static unsigned int Flip32(unsigned int value) { return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) | ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000); }
	static uint64_t Flip64(uint64_t value) 
	{ 
		return (value  & 0x00000000000000ff) << 56 | \
			   (value  & 0x000000000000ff00) << 40 | \
			   (value  & 0x0000000000ff0000) << 24 | \
			   (value  & 0x00000000ff000000) << 8  | \
			   (value  & 0x000000ff00000000) >> 8  | \
			   (value  & 0x0000ff0000000000) >> 24 | \
			   (value  & 0x00ff000000000000) >> 40 | \
			   (value  & 0xff00000000000000) >> 56;
	}
	static void Flip24(void *p, unsigned int val)
	{
		unsigned char *data = (unsigned char *)p;
		data[0] = val >> 16;
		data[1] = val >> 8;
		data[2] = val;
	}
private:
	
	bool  flip;
	const unsigned char*	_data;
	const unsigned char*	_end;
	const unsigned char*	_current;
	size_t			_size;

};




}
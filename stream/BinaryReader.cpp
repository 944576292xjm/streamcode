#include "BinaryReader.h"
#include <string.h>
namespace base {


BinaryReader::BinaryReader(const unsigned char* data, size_t size, bool bigEndian) :_data(data), _end(data + size), _current(data),
_size(size), flip(bigEndian)
{
}

BinaryReader::~BinaryReader()
{
}

unsigned int BinaryReader::Read(unsigned char* value, unsigned int size)
{
	unsigned int available = _end - _current;
	if (size > available)
	{
		memcpy(value, _current, available);
		_current += available;
		return available;
	}
	else
	{
		memcpy(value, _current, size);
		_current += size;
		return size;
	}
}

unsigned char BinaryReader::Read8()
{
	return _current != _end ? *_current++ : 0;
}

unsigned short BinaryReader::Read16()
{
	unsigned int value = 0;
	unsigned int ret = Read((unsigned char *)&value, 2);
	if (ret != 2)
	{
		return 0;
	}
	
	if (flip)
	{
		return Flip16(value);
	}
	return value;
}

unsigned int BinaryReader::Read24()
{
	unsigned int value = 0;
	unsigned int ret = Read((unsigned char *)&value, 3);
	if (ret != 3)
	{
		return 0;
	}

	if (flip)
	{
		return Flip24(value);
	}
	return value;
}

unsigned int BinaryReader::Read32()
{
	unsigned int value = 0;
	unsigned int ret = Read((unsigned char *)&value, 4);
	if (ret != 4)
	{
		return 0;
	}

	if (flip)
	{
		return Flip32(value);
	}
	return value;
}

uint64_t BinaryReader::Read64()
{
	uint64_t value = 0;
	uint64_t ret = Read((unsigned char *)&value, 8);
	if (ret != 8)
	{
		return 0;
	}

	if (flip)
	{
		return Flip64(value);
	}
	return value;
}

unsigned int BinaryReader::Available()
{
	return _current < _end ? _end - _current : 0;
}

unsigned int BinaryReader::Next(unsigned int count) 
{
	if (count > Available())
		count = Available();
	_current += count;
	return count;
}

const unsigned char*  BinaryReader::Current()
{
	return _current;
}

}
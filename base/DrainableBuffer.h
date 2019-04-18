#ifndef _BASE_DRIANABLE_BUFFER_H
#define _BASE_DRIANABLE_BUFFER_H

#include "base/TBuffer.h"
#include <string>
#include <stdio.h>

namespace base {


class DrainableBuffer : public TBuffer
{
public:
	DrainableBuffer(size_t size) : TBuffer(size) {
	
	}


	virtual ~DrainableBuffer() {
	
	}
	virtual bool Resize(size_t newSize);
	virtual bool AddReadPos(size_t n);
	virtual bool AddWritePos(size_t n);
	virtual bool Read(void *data, size_t n);
	//bool Peek8(unsigned char* data);
	//bool Peek16(unsigned short* data);
	//bool Peek24()
	//bool Peek32(unsigned int* data);

	virtual unsigned char  Peek8(size_t offset = 0);
	virtual unsigned short Peek16(size_t offset = 0);
	virtual unsigned int   Peek24(size_t offset = 0);
	virtual unsigned int   Peek32(size_t offset = 0);
	virtual uint64_t Peek64(size_t offset = 0);

	virtual unsigned char  Read8();
	virtual unsigned short Read16();
	virtual unsigned int   Read24();
	virtual unsigned int   Read32();
	virtual uint64_t Read64();


	virtual bool Peek(void* data, size_t n, size_t offset = 0);
	virtual bool Write(const void *dat, size_t n);
	bool Write(std::string str);
};


}

#endif
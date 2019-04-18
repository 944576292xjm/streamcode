#ifndef __BASE_RING_BUFFER_H
#define __BASE_RING_BUFFER_H

#ifdef __linux__
#include <sys/uio.h>

#elif __Win32__
#include "base/Socket.h"
#endif
#include <stdint.h>

#include <assert.h>

#include "base/TBuffer.h"

namespace base
{
	
class RingBuffer : public TBuffer
{
public:
	RingBuffer(size_t size) : TBuffer(size){}
	
	virtual ~RingBuffer(){}

	void He();


	void duandian();

public:
	virtual bool Resize(size_t newSize);
	virtual bool AddReadPos(size_t n);
	virtual bool AddWritePos(size_t n);
	virtual bool Read(void *data, size_t n);
	bool Read(TBuffer& buffer, size_t n);
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

	
	bool GetReadPos(struct iovec* first_read, struct iovec* second_read);
	bool GetWritePos(struct iovec* first_write, struct iovec* second_write);

	std::string ReadString(const std::string &spliter);
	std::string PeekString(const std::string &spliter, size_t pos = 0);



	static uint16_t Flip16(uint16_t value) { return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00); }
	static unsigned int Flip24(unsigned int value) { return ((value >> 16) & 0x000000FF) | (value & 0x0000FF00) | ((value << 16) & 0x00FF0000); }
	static unsigned int Flip32(unsigned int value) { return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00) | ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000); }
	static uint64_t Flip64(uint64_t value)
	{
		return (value & 0x00000000000000ff) << 56 | \
			(value & 0x000000000000ff00) << 40 | \
			(value & 0x0000000000ff0000) << 24 | \
			(value & 0x00000000ff000000) << 8 | \
			(value & 0x000000ff00000000) >> 8 | \
			(value & 0x0000ff0000000000) >> 24 | \
			(value & 0x00ff000000000000) >> 40 | \
			(value & 0xff00000000000000) >> 56;
	}

};
	
	
}


#endif
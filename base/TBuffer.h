#ifndef _BASE_BUFFER_H
#define _BASE_BUFFER_H

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
namespace base {

class TBuffer
{
public:
	TBuffer(size_t size)
		: buffer_(new char[size]),
		readPos_(0), writePos_(0),
		size_(0), capacity_(size)
	{
		assert(buffer_);
	}

	virtual ~TBuffer()
	{
		if (buffer_)
		{
			delete[] buffer_;
			buffer_ = NULL;
		}
	}

public:
	size_t Size() { return size_; }
	size_t Capacity() { return capacity_; }
	bool Full() { return capacity_ == size_; }
	bool Empty() { return size_ == 0; }
	char* GetWritePtr(){return buffer_ + writePos_;}
	char* GetReadPtr() { return buffer_ + readPos_; }

public:
	virtual bool Resize(size_t newSize) = 0;
	virtual bool AddReadPos(size_t n) = 0;
	virtual bool AddWritePos(size_t n) = 0;
	virtual bool Read(void *data, size_t n) = 0;
	//bool Peek8(unsigned char* data);
	//bool Peek16(unsigned short* data);
	//bool Peek24()
	//bool Peek32(unsigned int* data);

	virtual unsigned char  Peek8(size_t offset = 0) = 0;
	virtual unsigned short Peek16(size_t offset = 0) = 0;
	virtual unsigned int   Peek24(size_t offset = 0) = 0;
	virtual unsigned int   Peek32(size_t offset = 0) = 0;
	virtual uint64_t Peek64(size_t offset = 0) = 0;

	virtual unsigned char  Read8() = 0;
	virtual unsigned short Read16() = 0;
	virtual unsigned int   Read24() = 0;
	virtual unsigned int   Read32() = 0;
	virtual uint64_t Read64() = 0;


	virtual bool Peek(void* data, size_t n, size_t offset = 0) = 0;
	virtual bool Write(const void *dat, size_t n) = 0;

protected:
	char* buffer_;
	size_t writePos_;
	size_t readPos_;
	size_t size_;
	size_t capacity_;

};












}

#endif
#include "base/DrainableBuffer.h"
#include <string.h>

namespace base {




	bool DrainableBuffer::Resize(size_t newSize)
	{
	/*	if (newSize < size_)
		{
			return false;
		}

		char* newBuffer = new char[newSize];
		if (newBuffer == NULL)
		{
			int c = 0;
		}
		if (size_ > 0)
		{
			if (writePos_ <= readPos_)
			{
				::memcpy(newBuffer, buffer_ + readPos_, capacity_ - readPos_);
				::memcpy(newBuffer + capacity_ - readPos_, buffer_, writePos_);
			}
			else
			{
				::memcpy(newBuffer, buffer_ + readPos_, writePos_ - readPos_);
			}
		}

		delete[] buffer_;
		buffer_ = newBuffer;
		capacity_ = newSize;
		readPos_ = 0;
		writePos_ = size_;
		He();*/
		return true;
	}

	bool DrainableBuffer::AddWritePos(size_t n)
	{
		if (n > capacity_ - writePos_)
		{
			return false;
		}
		writePos_ += n;
		size_ += n;
		return true;
	}

	bool DrainableBuffer::AddReadPos(size_t n)
	{
		if (n > size_)
		{
			return false;
		}
		size_ -= n;
		readPos_ += n;
		return true;
	}

	bool DrainableBuffer::Read(void* data, size_t n)
	{

		Peek(data, n);

		readPos_ += n;
		size_ -= n;
		//
		return true;
	}




	bool DrainableBuffer::Peek(void *data, size_t n, size_t offset)
	{
		if (data == NULL || n + offset > size_)
		{
			return false;
		}

		memcpy(data, buffer_ + readPos_ + offset, n);
		return true;
	}

	unsigned char DrainableBuffer::Peek8(size_t offset)
	{
		unsigned char value = 0;
		bool ret = Peek(static_cast<void *>(&value), 1, offset);
		if (!ret)
		{
			return 0;
		}

		return value;
	}
	unsigned short DrainableBuffer::Peek16(size_t offset)
	{
		unsigned short value = 0;
		bool ret = Peek(static_cast<void *>(&value), 2, offset);
		if (ret != true)
		{
			return 0;
		}

		//	if (flip)
		{
			//		return Flip16(value);
		}
		return value;
	}
	unsigned int DrainableBuffer::Peek24(size_t offset)
	{
		unsigned int value = 0;
		unsigned int ret = Peek(static_cast<void *>(&value), 3, offset);
		if (ret != 3)
		{
			//		return 0;
		}

		//	if (flip)
		{
			//		return Flip24(value);
		}
		return value;
	}
	unsigned int DrainableBuffer::Peek32(size_t offset)
	{
		unsigned int value = 0;
		unsigned int ret = Peek(static_cast<void *>(&value), 4, offset);
		if (ret != 4)
		{
			//	return 0;
		}

		//	if (flip)
		{
			//		return Flip32(value);
		}
		return value;
	}

	uint64_t DrainableBuffer::Peek64(size_t offset)
	{
		uint64_t value = 0;
		uint64_t ret = Peek(static_cast<void *>(&value), 8, offset);
		if (ret != 4)
		{
			//	return 0;
		}

		return value;
	}

	unsigned char  DrainableBuffer::Read8()
	{
		unsigned char ret = Peek8();
		readPos_ += 1;
		size_ -= 1;
		return ret;
	}
	unsigned short DrainableBuffer::Read16()
	{
		unsigned short ret = Peek16();
		readPos_ += 2;
		size_ -= 2;
		return ret;
	}
	unsigned int   DrainableBuffer::Read24()
	{
		unsigned int ret = Peek24();
		readPos_ += 3;
		size_ -= 3;
		return ret;
	}
	unsigned int  DrainableBuffer::Read32()
	{
		unsigned char ret = Peek32();
		readPos_ += 4;
		size_ -= 4;
		return ret;
	}
	uint64_t DrainableBuffer::Read64()
	{
		uint64_t ret = Peek64();
		readPos_ += 8;
		size_ -= 8;
		return ret;
	}

	bool DrainableBuffer::Write(std::string str)
	{
		return Write(static_cast<const void *>(str.c_str()),str.size());
	}

	bool DrainableBuffer::Write(const void *data, size_t n)
	{
		if (data == NULL || n > capacity_ - size_)
		{
			return false;
			//	return false;
			if (n + size_ - capacity_ < capacity_)
			{
				Resize(capacity_ * 2);
			}
			else
			{
				Resize(n + size_);
			}
			int a = 0;
		}

		if (writePos_ + n < capacity_)
		{
			::memcpy(buffer_ + writePos_, data, n);
			writePos_ += n;
		}
		else
		{
			::memmove(buffer_, buffer_ + writePos_, size_);
			writePos_ = size_;
			readPos_ = 0;
			::memcpy(buffer_, data, n);
		}

		size_ += n;
		return true;
	}













}
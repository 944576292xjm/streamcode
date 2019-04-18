#include "base/RingBuffer.h"
#include <algorithm>

namespace base {

	static inline void* add_to_pointer(void* pointer, size_t amount) 
	{
		return static_cast<unsigned char *>(pointer) + amount;
	}


	void RingBuffer::He()
	{
		if (writePos_ > capacity_ || readPos_ > capacity_ || size_ > capacity_)
		{
			int a = 0;
		}
	}

	void RingBuffer::duandian()
	{
		if (this == NULL)
		{
			int c = 0;
		}
	}

bool RingBuffer::Resize(size_t newSize)
{
	if (newSize < size_)
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
	He();
	return true;
}
	
bool RingBuffer::AddWritePos(size_t n)
{
	if (n > capacity_ - size_)
	{
		He();
		return false;
	}
	size_ += n;
	if (writePos_ >= readPos_)
	{
		if (n < capacity_ - writePos_)
		{
			writePos_ += n;
		}
		else
		{
			writePos_ = n + writePos_ - capacity_;
		}
	}
	else
	{
		writePos_ += n;
	}
	He();
	return true;
}

bool RingBuffer::AddReadPos(size_t n)
{
	if (n > size_)
	{
		He();
		return false;
	}
	size_ -= n;
	if (writePos_ <= readPos_)
	{
		if (n < capacity_ - readPos_)
		{
			readPos_ += n;
		}
		else
		{
			readPos_ = n + readPos_ - capacity_;
		}
	}
	else
	{
		readPos_ += n;
	}
	He();
	return true;
}

bool RingBuffer::Read(void* data, size_t n)
{
	duandian();
	Peek(data, n);

	readPos_ = (readPos_ + n) % capacity_;
	size_ -= n;
//
	He();
	return true;
}

bool RingBuffer::Read(TBuffer& buffer, size_t n)
{
	duandian();
	if (n > size_)
	{
		return false;
	}
	if (readPos_ < writePos_)
	{
		buffer.Write(buffer_ + readPos_, n);
	}
	else
	{
		if (n <= capacity_ - readPos_)
		{
			buffer.Write(static_cast<const void *>(buffer_ + readPos_), n);
		}
		else
		{
			buffer.Write(static_cast<const void *>(buffer_ + readPos_), capacity_ - readPos_);
			buffer.Write(static_cast<const void *>(buffer_), n + readPos_ - capacity_);
		}
	}
	readPos_ = (readPos_ + n) % capacity_;
	size_ -= n;
	return true;
}

bool RingBuffer::Peek(void *data, size_t n, size_t offset)
{
	duandian();
	if(data == NULL || n + offset > size_)
	{
		He();
		return false;
	}
	
	if(readPos_ + offset >= writePos_)
	{
		if(n + offset <= capacity_ - readPos_)
		{
			::memcpy(data, buffer_ + readPos_ + offset, n);
	//		readPos_ += n;
		//	He();
		}
		else
		{
			::memcpy(data, buffer_ + readPos_ + offset, capacity_ - readPos_ - offset);
		//	::memcpy(data + capacity_ - readPos_, buffer_, n + readPos_ - capacity_);
			::memcpy(add_to_pointer(data, capacity_ - readPos_ - offset), buffer_, n + readPos_ + offset - capacity_);
		//	readPos_ = n + readPos_ - capacity_ ;
			He();
		}
	}
	else
	{
//		int a = *(buffer_ + readPos_ + offset);
		::memcpy(data, buffer_ + readPos_ + offset, n);
	//	readPos_ += n;
		He();
	}
	
//	readPos_ = (readPos_ + n) % capacity_;
//	size_ -= n;
//	He();
	return true;
}

unsigned char RingBuffer::Peek8(size_t offset)
{
	duandian();
	unsigned char value = 0;
	bool ret = Peek(static_cast<void *>(&value), 1, offset);
	if (!ret)
	{
		return 0;
	}

	return value;
}
unsigned short RingBuffer::Peek16(size_t offset)
{
	duandian();
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
unsigned int RingBuffer::Peek24(size_t offset)
{
	duandian();
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
unsigned int RingBuffer::Peek32(size_t offset)
{
	duandian();
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

uint64_t RingBuffer::Peek64(size_t offset)
{
	duandian();
	uint64_t value = 0;
	uint64_t ret = Peek(static_cast<void *>(&value), 8, offset);
	if (ret != 4)
	{
		//	return 0;
	}

//	if (flip)
	{
//		return Flip64(value);
	}
	return value;
}

std::string RingBuffer::ReadString(const std::string &spliter)
{
	duandian();
	std::string peek = PeekString(spliter);
	if (!peek.empty())
	{
		readPos_ = (readPos_ + peek.size()) % capacity_;
		size_ -= peek.size();
	}
	return peek;
}

std::string RingBuffer::PeekString(const std::string &spliter, size_t pos)
{
	duandian();
	if(pos > size_)
	{
		return "";
	}
	
	const char *str = std::search(buffer_ + readPos_+ pos, buffer_ + readPos_ + size_, spliter.c_str(), spliter.c_str() + spliter.size());
	size_t len = str - buffer_ - readPos_ - pos;
	if(len  > size_ || str > buffer_ + writePos_ || str < buffer_ + readPos_)
	{
		return "";
	}
	return std::string(buffer_ + readPos_ + pos, len);
}

unsigned char  RingBuffer::Read8()
{
	duandian();
	unsigned char ret = Peek8();
	readPos_ = (readPos_ + 1) % capacity_;
	size_ -= 1;
	return ret;
}
unsigned short RingBuffer::Read16()
{
	duandian();
	unsigned short ret = Peek16();
	readPos_ = (readPos_ + 2) % capacity_;
	size_ -= 2;
	return ret;
}
unsigned int   RingBuffer::Read24()
{
	duandian();
	unsigned int ret = Peek24();
	readPos_ = (readPos_ + 3) % capacity_;
	size_ -= 3;
	return ret;
}
unsigned int  RingBuffer::Read32()
{
	duandian();
	unsigned char ret = Peek32();
	readPos_ = (readPos_ + 4) % capacity_;
	size_ -= 4;
	return ret;
}
uint64_t RingBuffer::Read64()
{
	duandian();
	uint64_t ret = Peek64();
	readPos_ = (readPos_ + 8) % capacity_;
	size_ -= 8;
	return ret;
}

bool RingBuffer::Write(const void *data, size_t n)
{
	duandian();
	if(data == NULL || n > capacity_ - size_)
	{
		He();
	//	return false;
		if (n + size_ - capacity_ < capacity_)
		{
			Resize(capacity_ * 2);
		}
		else
		{
			Resize( n + size_);
		}
		He();
		int a = 0;
	}
	
	if(readPos_ <= writePos_)
	{
		if(n < capacity_ - writePos_)
		{
  			::memcpy(buffer_ + writePos_, data, n);
			writePos_ += n;

			He();
			int a = 0;
		}
		else
		{
			::memcpy(buffer_ + writePos_, data, capacity_ - writePos_);
			::memcpy(buffer_, add_to_pointer(const_cast<void *>(data), capacity_ - writePos_), n + writePos_ - capacity_);
			writePos_ = n + writePos_ - capacity_;
			He();
			int a = 0;
		}
	}
	else
	{
		::memcpy(buffer_ + writePos_, data, n);
		writePos_ += n;
		He();
		int a = 0; 
	}
	
	size_ += n;
	He();
	return true;
}

bool RingBuffer::GetReadPos(struct iovec* first_read, struct iovec* second_read)
{
	duandian();
	if(first_read == NULL || second_read == NULL
		|| size_ <= 0)
	{
		He();
		first_read->iov_base = 0;
		second_read->iov_len = 0;
		return false;
	}
	
	if(readPos_ < writePos_)
	{
		first_read->iov_base = buffer_ + readPos_;
		first_read->iov_len = writePos_ - readPos_;
		second_read->iov_base = NULL;
		second_read->iov_len = 0;
	}
	else
	{
		first_read->iov_base = buffer_ + readPos_;
		first_read->iov_len = capacity_ - readPos_;
		second_read->iov_base = buffer_;
		second_read->iov_len = writePos_;
	}
	He();
	return true;
}

bool RingBuffer::GetWritePos(struct iovec* first_write, struct iovec* second_write)
{
	duandian();
	if(first_write == NULL || second_write == NULL
		|| size_ == capacity_)
	{
		He();
		first_write->iov_len = 0;
		second_write->iov_len = 0;
		return false;
	}
	
	if(writePos_ < readPos_)
	{
		first_write->iov_base = buffer_ + writePos_;
		first_write->iov_len = readPos_ - writePos_;
		second_write->iov_base = NULL;
		second_write->iov_len = 0;
	}
	else
	{
		first_write->iov_base = buffer_ + writePos_;
		first_write->iov_len = capacity_ - writePos_;
		second_write->iov_base = buffer_;
		second_write->iov_len = readPos_;
	}
	He();
	return true;
}





}
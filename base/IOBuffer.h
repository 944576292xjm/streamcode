#pragma once
#include <string.h>
namespace base {

	
//	static int aa = 0;
class IOBuffer
{
public:
	IOBuffer(size_t size)
	{
		//assert(size > 0);
	//	aa++;
	//	printf("++++IOBuffer %d\n", aa);
		data_ = new char[size];
	}

	IOBuffer(char* data, size_t len) 
	{
		data_ = new char[len];
		len_ = len;
		::memcpy(data_, data, len);
	}

	IOBuffer(IOBuffer& other)
	{
		data_ = new char[other.len_];
		len_ = other.len_;
		::memcpy(data_, other.data_, other.len_);
	}
	IOBuffer(IOBuffer&& other)
	{
		data_ = other.data_;
		len_ = other.len_;
		other.data_ = NULL;
	}
	char* Data() { return data_; }
	size_t Size() { return len_; }
	virtual ~IOBuffer() 
	{
		if (data_)
		{
			delete data_;
		}
	}

private:
	char *data_;
	size_t len_;
};



typedef std::shared_ptr<IOBuffer> IOBufferPtr;

}
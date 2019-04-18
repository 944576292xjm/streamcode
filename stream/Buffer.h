#pragma once

#include <memory>

namespace base {

class Buffer
{
public:
	Buffer(unsigned int size = 400);
	Buffer(std::shared_ptr<Buffer> buffer);
	Buffer(unsigned char* buffer, unsigned int size);
	Buffer(Buffer &&buffer);
	virtual~Buffer();

	unsigned int size()  const;
	unsigned int capacity();
	unsigned int setsize(unsigned int size); 
	unsigned int Append(const void* data, unsigned int size);
	unsigned int Append(std::shared_ptr<Buffer> buffer);
	unsigned int MoveForward(unsigned int size);
	unsigned int Next(unsigned int next = 1);
	unsigned char* data();
	void   reset();
protected:
	unsigned char *_data;
	unsigned char *_current;
	unsigned char *_end;
	unsigned char *_last;
	unsigned int _size;
	unsigned int _capacity;
};


}
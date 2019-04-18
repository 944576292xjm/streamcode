#pragma once

#include "Buffer.h"
#include <string.h>
namespace base {

Buffer::Buffer(unsigned int size) :_size(0), _data(NULL), _current(NULL), _capacity(0)
{
	if (!size)
	{

	}

	_capacity = size;
	_data = new unsigned char[size];
	_current = _data;
	//::memset(_data, 0, _size);
}

Buffer::Buffer(std::shared_ptr<Buffer> buffer)
{
	_capacity = _size = buffer->size();
	_data = _current = new unsigned char[_capacity];
	memcpy(_data, buffer->data(), _size);
}

Buffer::Buffer(unsigned char* buffer, unsigned int size)
{
	_capacity = _size =  size;
	//_data = _current = buffer;
	_data = _current = new unsigned char[_capacity];
	memcpy(_data, buffer, _size);
}

Buffer::Buffer(Buffer &&buffer)
{
	_capacity = buffer._capacity;
	_data = buffer._data;
	_current = buffer._current;
	_size = buffer._size;
}

Buffer::~Buffer()
{
	if (_data)
		delete[] _data;
}

unsigned int Buffer::size()   const
{
	return _size;
}

unsigned int Buffer::capacity()
{
	return _capacity;
}

unsigned int Buffer::Append(std::shared_ptr<Buffer> buffer)
{
	return Append(buffer->data(), buffer->size());
}

unsigned int Buffer::MoveForward(unsigned int size)
{
	if (size >= _size)
	{
		return 0;
	}
	::memmove(_current, _current + size, _size - size);

	_size -= size;

	return size;
}

unsigned int Buffer::Next(unsigned int next)
{
	if (_size < next)
	{
		return 0;
	}
	_current += next;
	_size -= next;

	return next;
}

unsigned int Buffer::Append(const void* data, unsigned int size)
{
	if (!_data) // to expect null Buffer 
		return 0;

	//if ()
	unsigned int waste = _current - _data;

	unsigned int remain = _capacity - _size - waste;

	if (size <= remain)
	{
		::memcpy(_current + _size, data, size);
		_size += size;
		return size;
	}
	else if (size > remain && size < remain + waste)
	{
		//MoveForward(waste);
		::memmove(_data, _current, _size);
		_current = _data;
		::memcpy(_current + _size, data, size);
		_size += size;
		return size;
	}
	else
	{
		unsigned char *newData = new unsigned char[size + _size];

		if (newData == NULL)
		{
			return 0;
		}

		::memcpy(newData, _current, _size);
		::memcpy(newData + _size, data, size);

		_current = newData;
		//_capacity += size;
		_size += size;
		_capacity = _size;
		delete[] _data;
		_data = newData;

		return size;
	}
}

unsigned int Buffer::setsize(unsigned int size)
{
	if (size <= _capacity)
	{
		_size = size;
	}
	return size;
}


unsigned char *Buffer::data()
{
	return _current;
}

void   Buffer::reset()
{
	_current = _data;
	_size = 0;
}



}
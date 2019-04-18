#pragma once

#include <string>

namespace base {


class File
{
public:
	File(const char *path);
	~File();

	int Open();
	int Size();
	int Available();
	int Current();
	unsigned int Read(const unsigned char* buffer, unsigned int size);
private:
	std::string _path;
	FILE *fp;
	unsigned int _size;
};











}
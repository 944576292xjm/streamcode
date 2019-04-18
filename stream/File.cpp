#include "File.h"
#include <stdio.h>
namespace base {


File::File(const char *path) : _path(path), fp(NULL), _size(0)
{
}

File::~File()
{
	
}

//文本方式读取文件, 最主要的用处是一次读取一整句(以换行符'\n', 即二进制的换行标志"\r\n"结束), 
//方便用于特殊用处ReadString、fscanf(..., "%s", ...)之类, 每次读取的内容长度是不定的; 
//而二进制读取方式Read、fread等, 都是读取固定长度
//加上b是以二进制打开文件，不加b是文本形式打开文件
int File::Open()
{
	if (fp != NULL)
	{
		return -1;
	}

	fp = fopen(_path.c_str(), "rb");  
	if (NULL == fp)
	{
		return -1; 
	}

	fseek(fp, 0L, SEEK_END);
	_size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	//rewind(fp);

	return 0;
}

int File::Available()
{
	//unsigned int size = 0;
	//unsigned int current = ftell(fp);
//	fseek(fp, 0L, SEEK_END);
//	size = ftell(fp);
	if (fp == NULL)
	{
		return -1;
	}
	return _size - ftell(fp);
}

int File::Size()
{
	return _size;
}

int File::Current()
{
	if (fp == NULL)
	{
		return -1;
	}
	return ftell(fp);
}



unsigned int File::Read(const unsigned char* buffer, unsigned int size)
{
	int ret = fread((void *)buffer, 1, size, fp);
//	ret = ferror(fp);
	return ret;
	//return fread((void *)buffer, size, 1, fp);
}



}
#include "File.h"
#include <stdio.h>
namespace base {


File::File(const char *path) : _path(path), fp(NULL), _size(0)
{
}

File::~File()
{
	
}

//�ı���ʽ��ȡ�ļ�, ����Ҫ���ô���һ�ζ�ȡһ����(�Ի��з�'\n', �������ƵĻ��б�־"\r\n"����), 
//�������������ô�ReadString��fscanf(..., "%s", ...)֮��, ÿ�ζ�ȡ�����ݳ����ǲ�����; 
//�������ƶ�ȡ��ʽRead��fread��, ���Ƕ�ȡ�̶�����
//����b���Զ����ƴ��ļ�������b���ı���ʽ���ļ�
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
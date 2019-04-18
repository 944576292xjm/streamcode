#include <string>
#include <memory>
#include "Buffer.h"
#include "HttpRequest.h"

namespace base {


std::string HttpRequest::GetMethod()
{
	return _method;
}

std::string HttpRequest::GetUrl()
{
	return _url;
}

std::string HttpRequest::GetVersion()
{
	return _version;
}

std::string HttpRequest::GetHeader(const char *value)
{
	if (_filedmap.find(value) == _filedmap.end())
	{
		return "";
	}
	return _filedmap[value];
}

std::string HttpRequest::BuildHeader(unsigned int len)
{
/*	std::string data;
	data += "RTSP/1.0 200 OK\r\n";
	data += "CSeq: ";
	data += _filedmap["CSeq"];
	//data += "Content-Base: ";
	//data += _url;
	//data += "Content-Type: ";
	//data += "application/sdp";
//	data += "Content-Length: ";
//	data += len;
	return "";*/
	return "";
}



void HttpRequest::ParseLine(std::string& line)
{
	int pos = line.find(": ");
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos+ 2);
	if(!key.empty() && !value.empty())
	{
		_filedmap[key] = value;
	}
}

bool HttpRequest::ParseFirstLine(std::string& line)
{
	int start = 0;
	int pos; 
	
	pos = line.find(" ",start);
	if(pos == std::string::npos)
	{
		return false;
	}
	_method = line.substr(start, pos);

	start = pos + 1;
	pos = line.find(" ", start);
	if(pos == std::string::npos)
	{
		return false;
	}
	_url = line.substr(start, pos - start);

	start = pos + 1;

	_version = line.substr(start);	

	return true;
}

bool HttpRequest::ParseHead(std::string& head)
{
	int pos = head.find("\r\n");
	if(pos == std::string::npos)
	{
		return false;
	}

	std::string firstLine = head.substr(0, pos);
	if(!ParseFirstLine(firstLine))
	{
		return false;
	}

	pos += 2;
	
	unsigned int start = pos;
	while(start < head.size())
	{
		unsigned int pos = head.find("\r\n", start);
		if(pos == std::string::npos)
		{
			if (start < head.size())
			{
				std::string line = head.substr(start);
				ParseLine(line);
			}
			break;
		}
		std::string line = head.substr(start, pos - start);
		ParseLine(line);
		start = pos + 2;
	}

	return true;
}

bool HttpRequest::Parse(RingBuffer& buffer)
{
	int chunk_size = 0;
	int content_length = 0;
	while(true)
	{
		switch(httpStatus)
		{
			case kWaitingHeader:
			{
				std::string head;
				head = buffer.ReadString("\r\n\r\n");
				if (head.empty())
				{
					return false;
				}
				ParseHead(head);
				httpStatus = kWaitingBody;
				buffer.AddReadPos(strlen("\r\n\r\n"));
				break;
			}
			case kWaitingBody:
			{
				std::string string_content_length = GetHeader("Content-Length");
				std::string chunked("Transfer-Encoding");
				if(!string_content_length.empty())
				{
					content_length = std::stoi(string_content_length);
					httpStatus = kWaitingContentLength;
					break;
				}
				else if(chunked.compare("chunked") == 0)
				{
					httpStatus = kWaitingChunkSize;
					break;
				}
				httpStatus = kCompleted;
				break;
			}
			case kWaitingContentLength:
			{
				if(!buffer.Read(content_buffer_, content_length))
				{
					return false;
				}
				httpStatus = kCompleted;
				break;
			}
			case kWaitingChunkSize:
			{
				std::string string_chunk_size = buffer.ReadString("\r\n");
				if(string_chunk_size.empty())
				{
					return false;
				}
				chunk_size =  std::stoi(string_chunk_size);
				if(chunk_size == 0)
				{
					
				}
				buffer.AddReadPos(strlen("\r\n"));
				httpStatus = kWaitingChunkData;
				break;
			}
			case kWaitingChunkData:
			{
				if(buffer.Size() < chunk_size + 2)
				{
					return false;
				}
				buffer.Read(content_buffer_, chunk_size);
				buffer.AddReadPos(strlen("\r\n"));
				httpStatus = kWaitingChunkSize;
				break;
			}
			case kCompleted:
			{
				if(messageCallback_)
				{
					messageCallback_();
				}
				httpStatus = kWaitingHeader;
				break;
			}
			default:
			{
				return false;
			}
		}
	}
	return true;
}
	












}
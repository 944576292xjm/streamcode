#include <string>
#include <memory>
#include "Buffer.h"
#include "RtspRequest.h"

namespace base {


//std::string& GetMethod();
//std::string& GetUrl();
//std::string& GetVersion();


std::string FindField(const char* buf, const char* start, const char *end, int bufSize = 0) {
	if (bufSize <= 0){
		bufSize = strlen(buf);
	}
	const char *msg_start = buf, *msg_end = buf + bufSize;
	int len = 0;
	if (start != NULL) {
		len = strlen(start);
		msg_start = strstr(buf, start);
	}
	if (msg_start == NULL) {
		return "";
	}
	msg_start += len;
	if (end != NULL) {
		msg_end = strstr(msg_start, end);
		if (msg_end == NULL) {
			return "";
		}
	}
	return std::string(msg_start, msg_end);
}

RtspRequest::RtspRequest()
{
}

RtspRequest::~RtspRequest()
{
}

std::string RtspRequest::GetMethod()
{
	return _method;
}

std::string RtspRequest::GetUrl()
{
	return _url;
}

std::string RtspRequest::GetValue(const char *value)
{
	if (_filedmap.find(value) == _filedmap.end())
	{
		return "";
	}
	return _filedmap[value];
}

std::string RtspRequest::BuildHeader(unsigned int len)
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

int RtspRequest::GetTrackId()
{
	if (_url.empty())
	{
		return -1;
	}
	//unsigned int theTrackID;
	const char* start = NULL;
	if ((start = strstr(_url.c_str(), "trackID=")) || (start = strstr(_url.c_str(), "trackID")))
	{
		return ::strtol(start, NULL, 10);
	}
	else
	{
		return -1;
	}
}

void RtspRequest::ParseLine(std::string& line)
{
	int pos = line.find(": ");
	std::string key = line.substr(0, pos);
	std::string value = line.substr(pos+ 2);
	if(!key.empty() && !value.empty())
	{
		_filedmap[key] = value;
	}
}

bool RtspRequest::ParseFirstLine(std::string& line)
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

bool RtspRequest::ParseHead(std::string& head)
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

bool RtspRequest::Parse(RingBuffer& buffer)
{
	std::string head;
	head = buffer.PeekString("\r\n\r\n");
	if (head.empty())
	{
		return false;
	}
	ParseHead(head);
	buffer.AddReadPos(head.size() + 4);
	
	return true;
}
	












}
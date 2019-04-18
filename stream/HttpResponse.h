#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H
#include <string>
#include <map>
namespace base {


class HttpResponse
{
public:
	
	void AddHeader(std::string key, std::string value)
	{
		if(!key.empty() && !value.empty())
		{
			headers_[key] = value;
		}
	}
	void SetStatus(int status)
	{
		status_ = status;
	}

	/*void SetVersion();
	{
		if()
		
	}*/

	std::string BuildHeader()
	{
		std::string response;
	//	response.append("RTSP/1.0 ");
	////	response.append(std::to_string(status_));
	//	response.append("HTTP/1.1 ");
	//	response.append("HTTP/1.1 ");
	//	response.append(" ");
	//	response.append("OK");
		response.append("HTTP/1.1 200 OK\r\n");
		response.append("Cache-Control: no-cache\r\n");
		response.append("Content-Type: video/x-flv\r\n");
		response.append("Connection: Keep-Alive\r\n");
		response.append("Expires: -1\r\n");
//		response.append("\r\n");
		for (auto iter : headers_)
		{
			response.append(iter.first);
			response.append(": ");
			response.append(iter.second);
			response.append("\r\n");
		}
		response.append("\r\n");
		return response;
	}
	//void setBody(std::string body);
	
	
private:
	std::map<std::string, std::string> headers_;
	int status_;
};
		
}

#endif
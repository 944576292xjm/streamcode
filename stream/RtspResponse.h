#ifndef _HTTP_RESPONSE_H
#define _HTTP_RESPONSE_H
#include <string>
namespace base {


class RtspResponse
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

	std::string BuildHeader()
	{
		std::string response;
		response.append("RTSP/1.0 ");
		response.append(std::to_string(status_));
		response.append(" ");
		response.append("OK");
		response.append("\r\n");
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
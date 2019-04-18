#pragma once
#include <string>
#include <memory>
#include <functional>
#include <map>
#include "Buffer.h"
#include "base/RingBuffer.h"

namespace base {

	class HttpRequest
	{
	public:
		HttpRequest() : content_buffer_(1024 * 1024 * 10)
		{
		}
	//	~HttpRequest();

		std::string GetMethod();
		std::string GetUrl();
		std::string GetVersion();
		bool ParseFirstLine(std::string& line);
	
		void ParseLine(std::string& line);
		//	std::string& GetSessionId();
		//	std::string& GetCseq();
		bool Parse(RingBuffer& buffer);
		bool ParseHead(std::string& head);
		std::string BuildHeader(unsigned int len);
		std::string GetHeader(const char *);

		typedef std::function<void(void)> MessageCallback;

		void SetMessageCallback(MessageCallback callback)
		{
			messageCallback_ = std::move(callback);
		}

	private:
	
		typedef  enum
		{
			kWaitingHeader = 0,
			kWaitingBody = 1,
			kWaitingContentLength = 2,
			kWaitingChunkSize = 3,
			kWaitingChunkData = 4,
			kWaitingChunkTrailer = 5,
			kCompleted = 6,
		}ReceiveStatus;
		ReceiveStatus httpStatus = kWaitingHeader;
	private:

		std::string _method;
		std::string _url;
		std::string _version;
	//	std::string _accept;
	//	std::string _cseq;
	//	std::string _sessionId;
	//	std::string _transport;
		std::map<std::string, std::string> _filedmap;
		//std::string m_strContent;
		RingBuffer content_buffer_;
		MessageCallback messageCallback_;
	};





}
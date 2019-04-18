#pragma once
#include <string>
#include <memory>
#include <map>
#include "Buffer.h"
#include "base/RingBuffer.h"

namespace base {

	class RtspRequest
	{
	public:
		RtspRequest();
		~RtspRequest();

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
		int GetTrackId();
		std::string GetValue(const char *);

	private:
		typedef enum {
			ParingHead,
			ParingBody,
		}Status;
	
	private:

		std::string _method;
		std::string _url;
		std::string _version;
		std::string _accept;
		std::string _cseq;
		std::string _sessionId;
		std::string _transport;
		std::map<std::string, std::string> _filedmap;
		std::string m_strContent;

	};





}
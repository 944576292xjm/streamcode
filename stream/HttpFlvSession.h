#pragma once

#include <memory>
#include <functional>
#include "base/TcpConnection.h"
#include "HttpRequest.h"
#include "base/TBuffer.h"
#include "rtmp/RtmpSource.h"
#include "rtmp/MediaPacket.h"
#include "HttpResponse.h"
#include "FlvEncoder.h"
// <deque>

namespace base {

class HttpFlvSession : public std::enable_shared_from_this<HttpFlvSession>
{
public:
	HttpFlvSession(std::shared_ptr<TcpConnection> conn) : tcpconn_(conn)
	{
		request_.SetMessageCallback(std::bind(&HttpFlvSession::ProcessHttp, this));
	}
	
//	virtual ~HttpFlvSession();

	//typedef int(HttpFlvSession::*RtmpCmdFunction)(AMFReader &dec);
//	typedef std::function<int(AMFReader &dec)> RtmpCmdFunction;
	//typedef std::map<std::string, HttpFlvSession::RtmpCmdFunction> RtmpCmdMap;

	int OnRecv(std::shared_ptr<TcpConnection> conn);

	void ProcessHttp();

	void ReadFromSource(MediaPacket packet);

	//void ReadFromSource(std::shared_ptr<DrainableBuffer>);
	//void ReadFromSource(MediaPacket packet);
	//virtual int WriteVideo(std::shared_ptr<Packet> packet);

private:
	

	void SendTag(void* data, size_t size);

private:

	std::string app;
	std::string stream;

	HttpRequest request_;

	std::shared_ptr<FlvEncoder> flvEncoder_;

//	static RtmpCmdMap g_mapCmd;

//	std::shared_ptr<RtmpSource> m_pPublisherSrc;
//	std::shared_ptr<RtmpSource> m_pPublisherSrc;


	//´ý×ªÒÆ
//	std::shared_ptr<Packet>  _videoConfig;
//	std::deque<std::shared_ptr<Packet>> _videoPackets;

	TcpConnectionPtr tcpconn_;
};


}
#pragma once

#include <memory>
#include <functional>
#include "RTMP.h"
#include "Buffer.h"


#include "RtmpDecoder.h"

#include "AMFReader.h"
#include "RtmpSource.h"
#include "base/TcpConnection.h"
#include "MediaPacket.h"
#include <deque>

namespace base {

class RtmpSession : public std::enable_shared_from_this<RtmpSession>
{
public:
	RtmpSession(std::shared_ptr<TcpConnection> conn);
	virtual ~RtmpSession();

	typedef int(RtmpSession::*RtmpCmdFunction)(AMFReader &dec);
//	typedef std::function<int(AMFReader &dec)> RtmpCmdFunction;
	typedef std::map<std::string, RtmpSession::RtmpCmdFunction> RtmpCmdMap;

	virtual int OnRecv(std::shared_ptr<TcpConnection> conn);

	//void ReadFromSource(std::shared_ptr<DrainableBuffer>);
	void ReadFromSource(MediaPacket packet);
	//virtual int WriteVideo(std::shared_ptr<Packet> packet);
	virtual uint64_t Run();

	int RtmpConnect(AMFReader &reader);

	int RtmpCreateStream(AMFReader &reader);

	int RtmpPlay(AMFReader &reader);

	int RtmpCheckBw(AMFReader &reader);

	int RtmpReleaseStream(AMFReader &reader);

	int RtmpFCSubscribe(AMFReader &reader);

	int RtmpFCPublish(AMFReader &reader);

	int RtmpPublish(AMFReader &reader);



private:

	std::string app;

	RtmpDecoder _decoder;

	static RtmpCmdMap g_mapCmd;

	std::shared_ptr<RtmpSource> m_pPublisherSrc;
//	std::shared_ptr<RtmpSource> m_pPublisherSrc;


	//´ý×ªÒÆ
//	std::shared_ptr<Packet>  _videoConfig;
//	std::deque<std::shared_ptr<Packet>> _videoPackets;

	TcpConnectionPtr tcpconn;
};


}
#pragma once
#if 0
#include "Tiny.h"
#include "RTPStream.h"
#include <memory>
#include "UdpSocket.h"
#include "MediaConsumer.h"
#include "TcpSocket.h"
namespace Tiny {


class RTSPConsumer: public MediaConsumer
{
public:
	RTSPConsumer();
	~RTSPConsumer();

	int InitTcp(TcpSocket *socket);

	int InitAudio(const char* destip, unsigned int rtpPort, unsigned int rtcpPort, const char* localip = "192.168.0.101", unsigned int srtpPort = 5555, unsigned int srtcpPort = 5556);
	
	int InitVideo(const char* destip, unsigned int rtpPort, unsigned int rtcpPort, const char* localip = "192.168.0.101", unsigned int srtpPort = 5557, unsigned int srtcpPort = 555);

	virtual int WriteVideo(std::shared_ptr<Buffer> buffer);
	virtual int WriteAudio();
	virtual int WriteData();
	virtual int Write();
private:
	TcpSocket *pTcpRtp;

	UdpSocket *pVideoRtp;
	UdpSocket *pVideoRtcp;

	std::shared_ptr<RTPStream> _pAudio;
	std::shared_ptr<RTPStream> _pVideo;

};
#endif

	

}
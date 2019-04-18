#if 0
#include "RTSPConsumer.h"
#include "UdpSocket.h"
#include "RTP.h"

namespace Tiny {


RTSPConsumer::RTSPConsumer()
{
}

RTSPConsumer::~RTSPConsumer()
{
}

int RTSPConsumer::InitTcp(TcpSocket *socket)
{
	pTcpRtp = socket;
	return 0;
}


int RTSPConsumer::InitAudio(const char* destip, unsigned int rtpPort, unsigned int rtcpPort, const char* localip, unsigned int srtpPort, unsigned int srtcpPort)
{
	return 0;
}

int RTSPConsumer::InitVideo(const char* destip, unsigned int rtpPort, unsigned int rtcpPort, const char* localip,unsigned int srtpPort, unsigned int srtcpPort)
{
	pVideoRtp = new UdpSocket();

	pVideoRtp->SetPeerIp(destip);
	pVideoRtp->SetPeerPort(rtpPort);
	if(pVideoRtp->Open() < 0)
	{
		return -1;
	}
	if (pVideoRtp->SetReuseAddr() < 0)
	{
		return -1;
	}
//	if (pVideoRtp->SetNonBlocking() < 0)
	{
//		return -1;
	}
	if (pVideoRtp->Bind("192.168.0.101", srtpPort) < 0)
	{
		return -1;
	}
//	if (pVideoRtp->Connect(destip, rtpPort) < 0)
	{
//		return -1;
	}
	return 0;
}

int RTSPConsumer::WriteVideo(std::shared_ptr<Buffer> buffer)
{
	RTP::SendNaluPacket(buffer->data(), buffer->size(), pTcpRtp);
	//pVideoRtp->Send();
	return 0;
}

int RTSPConsumer::WriteAudio()
{
	return 0;
}

int RTSPConsumer::WriteData()
{
	return 0;
}

int RTSPConsumer::Write()
{
	return 0;
}

}

#endif
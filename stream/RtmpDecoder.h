#pragma once

#include <memory>
#include <map>
#include <atomic>
#include <vector>
#include "Buffer.h"
#include "base/RingBuffer.h"
#include "RTMP.h"
#include "base/TcpConnection.h"
#include "MediaPacket.h"

namespace base {

	class RtmpDecoder
	{
	public:
		RtmpDecoder(std::shared_ptr<TcpConnection> TcpConnection);
		~RtmpDecoder();

		enum RtmpState 
		{
			HANDSHAKE_C0C1,
			HANDSHAKE_C2,
			HANDSHAKE_DONE
		};

		std::vector<RtmpChannel> _packets;

		int decode(RingBuffer& Buffer);

		int SendResponse();

		int SendWinAckSize(unsigned int acksize);
		int SendChunkSize(unsigned int chunksize);
		int SendPeerWidth(unsigned int peerwidth);
		int SendOnBWDone();
		int SendBWResult(double id);
		int SendResult(bool ok, double id, AMFValue object, AMFValue value);
		int SendStreamBegin();
		int SendCreateRes();
		int SendStreamBegin0();
		int RtmpSampleAccess();
		int SendStatusReset(bool success);
		int SendStatus(AMFValue value);
		int SendStatusStart();
		int SendOnFcPublish(double id);
		int SendConnectStatus(bool result, double id);
		int SendFMLESmResult(double id);
		int SendcCreateStream(double id);
		int SendPubOnStatus(double id, bool ok);
//		int SendVideo(std::shared_ptr<Packet> packet);
		int SendVideo(MediaPacket packet);
	private:
		RtmpState _state;

		std::shared_ptr<TcpConnection> conn_;

		std::map<unsigned int, RtmpChannel>	channels_;

		//std::atomic<std::vector<RtmpChannel>> _packets;
	
		unsigned int _chunkSize;

		unsigned int _localchunkSize;

		int SendAgreement(AMFType type, std::string data);
		int SendRtmpProtol(AMFType type, unsigned int chunkid, unsigned int streamid, const char *data, unsigned int datasize, unsigned int time = 0);
	};

	
}
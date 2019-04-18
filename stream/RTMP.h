#pragma once
#include "WinSock2.h"
#include <stdlib.h>
#include "AMF.h"
#include "Packet.h"
#include "base/IOBuffer.h"
#include "base/DrainableBuffer.h"


namespace base {

#define HANDSHAKE_C0_LEN 1
#define HANDSHAKE_C1_LEN 1536
#define HANDSHAKE_C2_LEN 1536
#define RANDOM_LEN 1528
#define RTMP_VERSION 3

#define CONTROL_STREAM_ID  0

#define CHUNK_AGREEMENT_CHUNKID	2 /*服务器像客户端发出请求时的chunkID*/
#define CHUNK_CLIENT_CONTROL_STREAM1	3 /*客户端在createStream前,向服务器发出请求的chunkID*/
#define CHUNK_CLIENT_CONTROL_STREAM2	4 /*客户端在createStream后,向服务器发出请求的chunkID*/
#define CHUNK_VIDEO						5 /*视频chunkID*/
#define CHUNK_AUDIO						6 /*音频chunkID*/

#define CONTROL_STREAM_BEGIN		0
#define CONTROL_STREAM_EOF			1
#define CONTROL_STREAM_DRY			2
#define CONTROL_SETBUFFER			3
#define CONTROL_STREAM_ISRECORDED	4
#define CONTROL_PING_REQUEST		6
#define CONTROL_PING_RESPONSE		7



#define RTMP_AMF0_COMMAND_CONNECT               "connect"
#define RTMP_AMF0_COMMAND_CREATE_STREAM         "createStream"
#define RTMP_AMF0_COMMAND_CLOSE_STREAM          "closeStream"
#define RTMP_AMF0_COMMAND_PLAY                  "play"
#define RTMP_AMF0_COMMAND_PAUSE                 "pause"
#define RTMP_AMF0_COMMAND_ON_BW_DONE            "onBWDone"
#define RTMP_AMF0_COMMAND_ON_STATUS             "onStatus"
#define RTMP_AMF0_COMMAND_RESULT                "_result"
#define RTMP_AMF0_COMMAND_ERROR                 "_error"
#define RTMP_AMF0_COMMAND_RELEASE_STREAM        "releaseStream"
#define RTMP_AMF0_COMMAND_FC_PUBLISH            "FCPublish"
#define RTMP_AMF0_COMMAND_UNPUBLISH             "FCUnpublish"
#define RTMP_AMF0_COMMAND_PUBLISH               "publish"
#define RTMP_AMF0_DATA_SAMPLE_ACCESS            "|RtmpSampleAccess"

	// FMLE
#define RTMP_AMF0_COMMAND_ON_FC_PUBLISH         "onFCPublish"
#define RTMP_AMF0_COMMAND_ON_FC_UNPUBLISH       "onFCUnpublish"

#define RTMP_SIG_FMS_VER                        "3,5,3,888"
#define RTMP_SIG_AMF0_VER                       0.0

#define RTMP_SIG_AMF3_VER                       3.0



#define PEERWIDTH_DEFAULT 5000000
#define WIN_ACK_SIZE_DEFAULT 5000000

enum {
	RTMP_DEFAULT_CHUNKSIZE = 128,
	RTMP_DEFAULT_WIN_ACKSIZE = 131072 // TODO default value?
};

struct RtmpHeader {
	unsigned char flags;
	unsigned char timeStamp[3];
	unsigned char bodySize[3];
	unsigned char typeId;
	unsigned char streamId[4]; /* Note, this is little-endian while others are BE */
};


struct RtmpChannel 
{
public:
	RtmpChannel() : headLen(0), channelId(-1), time(0), absoluteTime(0), bodySize(0), type(AMF_TYPE_EMPTY), streamId(0xffffffff){}
	RtmpChannel(RtmpChannel &&packet)
	{
		headLen = packet.headLen;
		channelId = packet.channelId;
		time = packet.time; 
		absoluteTime = packet.absoluteTime;
		bodySize = packet.bodySize; 
		type = packet.type;
		streamId = packet.streamId;
		iobuf = std::move(packet.iobuf);
		//_pBuffer = packet._pBuffer;
	}

	
	RtmpChannel(RtmpChannel &packet)
	{
		headLen = packet.headLen;
		channelId = packet.channelId;
		time = packet.time;
		absoluteTime = packet.absoluteTime;
		bodySize = packet.bodySize;
		type = packet.type;
		streamId = packet.streamId;
		iobuf = packet.iobuf;
		//packet.reset();
	}
	//std::shared_ptr<IOBuffer> iobuf;
	std::shared_ptr<DrainableBuffer> iobuf;
	unsigned char headLen;
	unsigned char channelId;
	unsigned int time;
	unsigned int absoluteTime;
	unsigned int bodySize;
	AMFType type;
	unsigned int streamId;
	//std::shared_ptr<Buffer> _pBuffer;

/*	void reset()
	{
		reset(NULL);
		headLen = bodySize = time = 0;
		channelId = -1;
		type = AMF_TYPE_EMPTY;
		streamId = 0xffffffff;
	}*/

};

class RtmpHandshake {
public:
	RtmpHandshake(unsigned int _time, unsigned char *_random = nullptr) {
		_time = htonl(_time);
		memset(zero, 0, sizeof(zero));
		memcpy(timeStamp, &_time, 4);
		if (!_random) {
			random_generate((char *)random, sizeof(random));
		}
		else {
			memcpy(random, _random, sizeof(random));
		}
	}
	unsigned char timeStamp[4];
	unsigned char zero[4];
	unsigned char random[RANDOM_LEN];
	void random_generate(char* bytes, int size) {
		static char cdata[] = { 0x73, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x2d, 0x72,
			0x74, 0x6d, 0x70, 0x2d, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72,
			0x2d, 0x77, 0x69, 0x6e, 0x6c, 0x69, 0x6e, 0x2d, 0x77, 0x69,
			0x6e, 0x74, 0x65, 0x72, 0x73, 0x65, 0x72, 0x76, 0x65, 0x72,
			0x40, 0x31, 0x32, 0x36, 0x2e, 0x63, 0x6f, 0x6d };
		for (int i = 0; i < size; i++) {
			bytes[i] = cdata[rand() % (sizeof(cdata) - 1)];
		}
	}
};

}
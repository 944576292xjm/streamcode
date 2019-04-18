#ifndef _FLV_ENCODER_H
#define _FLV_ENCODER_H

#include "rtmp/MediaPacket.h"
#include "BinaryReader.h"
#include <functional>
namespace base {
	
	
struct RtmpTagHeader {
    unsigned char type = 0;
    unsigned char data_size[3] = {0};
    unsigned char timestamp[3] = {0};
    unsigned char timestamp_ex = 0;
    unsigned char streamid[3] = {0}; 
};

class FlvEncoder
{
public:
	typedef std::function<void(void*, size_t)> SendCallback;


	void WriteHeader()
	{
		char FLV_HEADER_BUF[] = { 0x46, 0x4c, 0x56, 0x01, 0x05, 0x0, 0x0, 0x0, 0x09 };
		sendCallback_(FLV_HEADER_BUF, 9);
	}

	void WriteFlvTag(MediaPacket packet)
	{
		unsigned int size = BinaryReader::Flip32(preTagSize_);
		sendCallback_((void *)&size, sizeof(size));

		RtmpTagHeader header;
		if (packet.GetFrameType() == MediaPacket::VIDEO)
		{
			header.type = FLV_TAG_VIDEO;
		}
		else if (packet.GetFrameType() == MediaPacket::VIDEO)
		{
			header.type = FLV_TAG_AUDIO;
		}
		else if (packet.GetFrameType() == MediaPacket::METADATA)
		{
			header.type = FLV_TAG_META;
		}
		//header.type = packet.type;
	//	BinaryReader::Flip24(header.data_size, packet.size);
		BinaryReader::Flip24(header.data_size, packet.GetFrameSize());
		BinaryReader::Flip24(header.timestamp, packet.GetTimeStamp() & 0xFFFFFF);
		header.timestamp_ex = ((packet.GetTimeStamp() >> 24) & 0xFF);
		sendCallback_(&header, sizeof(header));
		sendCallback_(packet.buffer->GetReadPtr(), packet.buffer->Size());

		preTagSize_ = packet.GetFrameSize() + sizeof(RtmpTagHeader);
	}

	void SetSendCallback(SendCallback callback)
	{
		sendCallback_ = std::move(callback);
	}
	
	
	
private:
	unsigned int preTagSize_ = 0;
	SendCallback sendCallback_;
	
	
	
};

}
#endif
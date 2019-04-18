#pragma once
#include "Media.h"
#include <memory>
#include <functional>
#include "base/DrainableBuffer.h"

namespace base {
	class MediaPacket;
typedef std::function<void(MediaPacket)> PacketReadCallback;

class MediaPacket
{
public:
	typedef enum {
		VIDEO = 0,
		VIDEO_CONFIG = 1,
		AUDIO = 2,
		AUDIO_CONFIG = 3,
		METADATA = 4,
		UNKNOWN = 5,
	}FrameType;

	FrameType GetFrameType() { return type_; }
	void SetFrameType(FrameType type) { type_ = type; }



	bool IsKeyFrame() {return isKeyFrame_;}
	void SetKeyFrame(bool isKeyFrame = true) { isKeyFrame_ = isKeyFrame; }

	unsigned int GetTimeStamp() { return timestamp_; }
	void SetTimeStamp(unsigned int timestamp) { timestamp_ = timestamp; }

	size_t GetFrameSize() { return buffer->Size(); }

	bool Empty() { return buffer == NULL; }

	std::shared_ptr<DrainableBuffer> buffer;
	//std::shared_ptr<DrainableBuffer> 

//for RTMP only
	void SetChannelId(unsigned char channelId) { channelId_ = channelId; }
	unsigned char GetChannelId() { return channelId_; }

	void SetStreamId(unsigned int streamId) { streamId_ = streamId; }
	unsigned int GetStreamId() { return streamId_; }
private:

//	unsigned char type;
//	unsigned char frame;
	FrameType type_ = UNKNOWN;
	bool isKeyFrame_ = false;
	unsigned int timestamp_ = 0;

//RTMP
	unsigned char channelId_ = 0;
	unsigned int streamId_ = 0;
	//unsigned int timeoffset;
	//unsigned char channelId;
//	unsigned int streamId;
	

};

}


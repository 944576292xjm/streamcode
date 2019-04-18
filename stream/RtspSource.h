#pragma once

#include <vector>
#include <map>
//#include <mutex>
#include "Buffer.h"
#include <string>
#include <functional>
#include "MediaPacket.h"
#include "base/Mutex.h"
#include <deque>
#include <deque>

namespace base {


class RtspSource
{
public:

	//	typedef std::function<void(std::shared_ptr<RtmpChannel>)> RtmpReadCallback;
	typedef std::function<void(MediaPacket)> RtspReadCallback;

	RtspSource() {}
	~RtspSource() {}

	void SetReadCallback(RtspReadCallback cb)
	{
		base::AutoLock lock(callbackMutex);
		callbackQueue_.push_back(cb);
	}

	/*	void SetVideoConfig(std::shared_ptr<RtmpChannel> packet)
	{
	_videoConfig = packet;
	}*/

	void PushPacketConfig(MediaPacket packet)
	{
		videoConfig = packet;
	}

	MediaPacket GetVideoConfig()
	{
		return videoConfig;
	}

	//int PushPacket(std::shared_ptr<RtmpChannel> packet);
	int PushPacket(MediaPacket packet);


private:
	base::Mutex callbackMutex;
	std::deque<RtspReadCallback> callbackQueue_;



public:

	static  std::shared_ptr<RtspSource>  FindMediaSource(std::string strApp);
	static  std::shared_ptr<RtspSource>  CreateMediaSource(std::string strApp);

public:
	static std::map<std::string, std::shared_ptr<RtspSource>> rtspSrcMap_;

	static base::Mutex srcMutex;

private:
	bool enable_gop_cache;
	//	std::shared_ptr<RtmpChannel> _videoConfig;
	MediaPacket videoConfig;
	//std::shared_ptr<RtmpChannel> _audioConfig;

	//std::deque<std::shared_ptr<RtmpChannel>> gop_cache;
	std::deque<MediaPacket> gop_cache;
};



	


}
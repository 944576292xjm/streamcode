#pragma once

#include <memory>
#include <string>
#include <map>
#include <deque>
#include <vector>
#include <atomic>
#include "base/Mutex.h"
#include "base/DrainableBuffer.h"
#include <functional>
#include "RTMP.h"
//#include "MediaPacket.h"
#include "MediaPacket.h"
#include "GopCache.h"

namespace base {

	class RtmpSource
	{
	public:

		//typedef std::function<void(std::shared_ptr<>)> PacketReadCallback;
		typedef std::function<void(MediaPacket)> RTMPPacketReadCallback;
		RtmpSource() {}
		~RtmpSource() {}

		bool StartRead(RTMPPacketReadCallback readCallback);
		int PushPacket(MediaPacket packet);
		void PushPacketConfig(MediaPacket packet);
		void PushAudioConfig(MediaPacket packet);

	private:
		base::Mutex callbackMutex;
		std::deque<RTMPPacketReadCallback> callbackQueue_;



	public:

		static  std::shared_ptr<RtmpSource>  FindMediaSource(std::string strApp);
		static  std::shared_ptr<RtmpSource>  CreateMediaSource(std::string strApp);

	public:
		static std::map<std::string, std::shared_ptr<RtmpSource>> rtmpSrcMap_;

		static base::Mutex srcMutex;

	private:
		bool enable_gop_cache;
	//	std::shared_ptr<RtmpChannel> _videoConfig;
	//	std::atomic<MediaPacket> videoConfig;
		MediaPacket videoConfig;
	//	MediaPacket videoConfig;
		MediaPacket audioConfig;
//		std::shared_ptr<RtmpChannel> _audioConfig;

		//std::deque<std::shared_ptr<RtmpChannel>> gop_cache;
//		std::vector<RTMPPacketReadCallback> gopCache;
		GopCache gopCache_;
	};

}
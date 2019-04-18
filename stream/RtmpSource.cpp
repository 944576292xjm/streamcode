#include "RtmpSource.h"

namespace base {

	std::map<std::string, std::shared_ptr<RtmpSource>> RtmpSource::rtmpSrcMap_;
	base::Mutex RtmpSource::srcMutex;

	std::shared_ptr<RtmpSource>  RtmpSource::FindMediaSource(std::string strApp)
	{
		base::AutoLock lock(srcMutex);
		auto source = rtmpSrcMap_.find(strApp);
		if (source == rtmpSrcMap_.end())
		{
			return NULL;
		}
		return source->second;
	}
	std::shared_ptr<RtmpSource>  RtmpSource::CreateMediaSource(std::string strApp)
	{
		base::AutoLock lock(srcMutex);
		auto source = rtmpSrcMap_.find(strApp);
		if (source == rtmpSrcMap_.end())
		{
			std::shared_ptr<RtmpSource> source(new RtmpSource());
			rtmpSrcMap_.emplace(strApp, source);
			return source;
		}
		return source->second;
	}

	bool RtmpSource::StartRead(RTMPPacketReadCallback readCallback)
	{
		if (videoConfig.Empty())
		{
			return false;
		}

		readCallback(videoConfig);
		gopCache_.GetGopCache(readCallback);
		//		readCallback(audioConfig);
		base::AutoLock lock(callbackMutex);
		callbackQueue_.push_back(readCallback);
		return true;
	}

	void  RtmpSource::PushPacketConfig(MediaPacket packet)
	{
		videoConfig = packet;
	}

	void  RtmpSource::PushAudioConfig(MediaPacket packet)
	{
		audioConfig = packet;
	}


	int RtmpSource::PushPacket(MediaPacket packet)
		//int RtmpSource::PushPacket(std::shared_ptr<RtmpChannel> packet)
	{
		//std::lock_guard<std::recursive_mutex> lock(gMtxSrc);

		decltype(callbackQueue_) copy;
		{
			base::AutoLock lock(callbackMutex);
			copy = callbackQueue_;
		}

		
		//	Lock lock(&_cacheMux);
		

		gopCache_.Cache(packet);
//		gop_cache.push_back(packet);
	

	//	printf("fasong le video---\n");
		for (auto iter : copy)
		{
			iter(packet);
		}

		{
			//	base::AutoLock lock(callbackMutex);
			//	callbackQueue_.push(cb);
		}

		return 0;

	}
		
}

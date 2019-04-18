#pragma once

#include "RtspSource.h"
#include <map>

namespace base {

std::map<std::string, std::shared_ptr<RtspSource>> RtspSource::rtspSrcMap_;
base::Mutex RtspSource::srcMutex;

std::shared_ptr<RtspSource>  RtspSource::FindMediaSource(std::string strApp)
{
	base::AutoLock lock(srcMutex);
	auto source = rtspSrcMap_.find(strApp);
	if (source == rtspSrcMap_.end())
	{
		return NULL;
	}
	return source->second;
}
std::shared_ptr<RtspSource>  RtspSource::CreateMediaSource(std::string strApp)
{
	base::AutoLock lock(srcMutex);
	auto source = rtspSrcMap_.find(strApp);
	if (source == rtspSrcMap_.end())
	{
		std::shared_ptr<RtspSource> source(new RtspSource());
		rtspSrcMap_.emplace(strApp, source);
		return source;
	}
	return source->second;
}

int RtspSource::PushPacket(MediaPacket packet)
//int RtmpSource::PushPacket(std::shared_ptr<RtmpChannel> packet)
{
	//std::lock_guard<std::recursive_mutex> lock(gMtxSrc);

	decltype(callbackQueue_) copy;
	{
		base::AutoLock lock(callbackMutex);
		copy = callbackQueue_;
	}

	{
		//	Lock lock(&_cacheMux);
		if (packet.GetFrameType() == FLV_FRAME_KEY)
		{
			printf("send key frame--\n");
			//			gop_cache.clear();
		}
		//		gop_cache.push_back(packet);
	}

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
#if 0 
static int num = 0;
int RtspSource::WriteVideo(std::shared_ptr<Buffer> packet)
{
	if (num == 0)
	{
		sps = std::string((char *)packet->data(), packet->size());
		num++;
		//return 0;
	}
	else if (num == 1)
	{
		pps = std::string((char *)packet->data(), packet->size());
		num++;
	//	return -1;
	}
	else if (num == 2)
	{
		num++;
		//return 0;
	}
	//std::lock_guard<std::recursive_mutex> lock(gMtxSrc);
	for (auto it : _consumers)
	{
		it->WriteVideo(packet);
	}

	return 0;
}

#endif
}
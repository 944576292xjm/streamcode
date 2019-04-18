
#include "GopCache.h"
#include "MediaPacket.h"

namespace base {
	

void GopCache::Cache(MediaPacket packet)
{
	if(packet.GetFrameType() == MediaPacket::VIDEO)
	{
		cacheVideoCount++;
//		audioAfterVideoCount = 0;
	}
	
	if(cacheVideoCount == 0)
	{
		return;
	}
	
	/*if(packet.IsAudio())
	{
	//	audioAfterVideoCount++;
	}*/
	
/*	if(audioAfterVideoCount > SRS_PURE_AUDIO_GUESS_COUNT)
	{
		
	}from srs*/ 
	
	if(packet.GetFrameType() == MediaPacket::VIDEO && packet.IsKeyFrame())
	{
	//	printf("timestamp is %d\n", packet.GetTimeStamp());
		clear();
		cacheVideoCount = 1;
	}
	
	packetQueue.push_back(packet);
}

void GopCache::clear()
{
	packetQueue.clear();
	cacheVideoCount = 0;
}



void  GopCache::GetGopCache(PacketReadCallback callback)
{
	for(auto packet : packetQueue)
	{
		callback(packet);
	}
	
}
	

	
}


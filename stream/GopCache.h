#ifndef _GOP_CACHE_H
#define _GOP_CACHE_H

#include <vector>
#include "MediaPacket.h"

namespace base {
	
class GopCache
{
public:
	void Cache(MediaPacket packet);
	void clear();
	void GetGopCache(PacketReadCallback callback);
	
private:
	int cacheVideoCount;
	std::vector<MediaPacket> packetQueue;
};
		
}


#endif
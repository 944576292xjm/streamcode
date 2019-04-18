#include "AVCDemuxer.h"

namespace base
{
	
int AVCDemuxer::DemuxIbmfFormat()
{
	unsigned int len = buffer.Size();
	
	for(int i = 0; i < len;)
	{
		unsigned nalu_len = Read32();
		buffer(auto, nalu_len);
		cache
	}
	
}
	
int AVCDemuxer::DemuxVideo()
{
	unsigned char codec = Read8();
	unsigned char codec_id = codec & 0x0f;
	unsigned char codec_type = (codec >> 4) & 0x0f;
	
	unsigned char nalu_type = Read8();
	unsigned int composition_time = Read24();
	
	if(nalu_type == CodecVideoAVCTypeNALU)
	{
		unsigned int len = buffer.Size();
	
		for(int i = 0; i < len;)
		{
			unsigned nalu_len = Read32();
			buffer(auto, nalu_len);
			cache
		}
	}
}
	
	
	
	
	
	
	
	
}
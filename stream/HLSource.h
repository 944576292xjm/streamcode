#ifndef _HLS_SOURCE_H
#define _HLS_SOURCE_H

#include "MediaPacket.h"
#include "RtmpSource.h"
#include "HlsMixer.h"
#include "AVC.h"


namespace base
{




class HLSource
{
public:
	static  std::shared_ptr<RtmpSource> FindMediaSource(std::string app);
	static  std::shared_ptr<RtmpSource>  CreateMediaSource(std::string strApp);

//	VideoCache video;


	DrainableBuffer video_cache;

	std::string sps;
	std::string pps;
	uint64_t pts;
	uint64_t dts;

	void srs_avc_insert_aud(DrainableBuffer &buffer, bool& header_inserted)
	{
		static unsigned char fresh_nalu_header[] = { 0x00, 0x00, 0x00, 0x01 };
		static unsigned char cont_nalu_header[] = { 0x00, 0x00, 0x01 };

		if (!header_inserted) {
			header_inserted = true;
			//payload->append((const char*)fresh_nalu_header, 4);
			buffer.Write(fresh_nalu_header, 4);
		}
		else {
			buffer.Write(fresh_nalu_header, 4);
		}
	}

	void CacheVideo(MediaPacket& packet)
	{
		DrainableBuffer buffer = *packet.buffer;
		unsigned char codec = buffer.Read8();
		unsigned char codec_id = codec & 0x0f;
		unsigned char codec_type = (codec >> 4) & 0x0f;

		unsigned char nalu_type = buffer.Read8();
		unsigned int composition_time = buffer.Read24();
		bool header_inserted = false;
	//	if (nalu_type == CodecVideoAVCType)
		{
	//		unsigned int len = buffer.Size();
			static unsigned char default_aud_nalu[] = { 0x09, 0xf0 };
			srs_avc_insert_aud(video_cache, header_inserted);
			video_cache.Write(default_aud_nalu, 2);
		//	for (int i = 0; i < len;)
			while(buffer.Size())
			{
			//	unsigned char* video = buffer.GetReadPtr();
				unsigned nalu_len = buffer.Read32();
				unsigned char nalu_type = buffer.Peek8() & 0x1f;
				if (nalu_type == AvcNaluTypeIDR)
				{
					//insert sps pps
					if (!sps.empty())
					{
						srs_avc_insert_aud(video_cache, header_inserted);
						video_cache.Write(sps);
					}
					if (!pps.empty())
					{
						srs_avc_insert_aud(video_cache, header_inserted);
						video_cache.Write(pps);
					}
				}
			//	buffer(auto, nalu_len);
				srs_avc_insert_aud(video_cache, header_inserted);
				video_cache.Write(buffer.GetReadPtr(), nalu_len);
				buffer.AddReadPos(nalu_len);
			}
		}
	}




	int ReapSegment()
	{
		segment_close();
	}
	
	void OnRecv(MediaPacket packet)
	{
		if (packet.GetFrameType() == MediaPacket::VIDEO_CONFIG)
		{
			packet.buffer->AddReadPos(6);
			AVC::ReadVideoConfig((const unsigned char *)(packet.buffer->GetReadPtr()), packet.buffer->Size(), sps, pps);
		}
		else if(packet.GetFrameType() == MediaPacket::VIDEO)
		{
			CacheVideo(packet);
			if (muxer_.SegmentOverFlow() && packet.IsKeyFrame())
			{
				ReapSegment();
			}

			muxer_.FlushVideo(packet);
		}
		
	//	muxer->Flush();
	}

	HlsMuxer muxer_;
};
	
	
}


#endif
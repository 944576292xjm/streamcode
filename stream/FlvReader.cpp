#include "FlvReader.h"
#include "BinaryReader.h"
#include "Media.h"
#include "AVC.h"
#include "File.h"
#include "BinaryReader.h"
#include <algorithm>

namespace base {

static size_t kMaxBufferSizeBytes = 4 * 1024 * 1024L;


bool FlvReader::Open()
{
	if (file_.Open() < 0)
	{
		return false;
	}
//	WorkPool::SingleInstance();
	WorkPool::SingleInstance()->PostTask(std::bind(&FlvReader::ReadHeader, this));
	return true;
}

void FlvReader::DecodeHead(const unsigned char*data, size_t size)
{
	BinaryReader reader(data, size);
	if (reader.Available() < FLV_HEADER_LENGTH)
	{
		return;
	}

	if (reader.Read8() == 0x46 && reader.Read8() == 0x4c && reader.Read8() == 0x56) //"FLV"
	{
		//reader.Next(2);
		version_ = reader.Read8();
		unsigned char info = reader.Read8();
		hasVideo_ = info & 1 << 7;
		hasAudio_ = info & 1 << 5;
		unsigned headlen = reader.Read32();
		if(headlen > 9)
		{
			reader.Next(headlen - 9);
		}
		DocodeChunk(data + headlen, size - headlen);
	} 
}


void FlvReader::DocodeChunk(const unsigned char*data, size_t size)
{
	size_t offset = 0;
	while (offset < size)
	{
		const unsigned char* input_bytes;
		size_t input_size;
		if (data_buffer_.empty())
		{
			input_bytes = data + offset;
			input_size = size - offset;
			offset += input_size;
		}
		else
		{

			size_t fill_buffer_size = min(kMaxBufferSizeBytes - data_buffer_.size(), size - offset);

			data_buffer_.insert(data_buffer_.end(), data + offset,
				data + offset + fill_buffer_size);

			input_bytes = &data_buffer_[0];
			input_size = data_buffer_.size();
			offset += fill_buffer_size;
		}

		size_t decoded_size = DecodeChunkInternal(input_bytes, input_size);
		if (decoded_size == 0 && offset < size)
		{
			//kerror;
		}

		size_t undecoded_size = input_size - decoded_size;
		if (undecoded_size == 0)
		{
			data_buffer_.clear();
		}
		else
		{
			if (data_buffer_.empty())
			{
				if (undecoded_size > kMaxBufferSizeBytes)
				{
					//kerror;
				}
				data_buffer_.reserve(undecoded_size);
				data_buffer_.assign(data + offset - undecoded_size, data + offset);
			}
			else if (decoded_size != 0)
			{
				data_buffer_.erase(data_buffer_.begin(), data_buffer_.begin() + static_cast<ptrdiff_t>(decoded_size));
			}
		}
	}
	ReadNextChunk();
}
static int a = 0;
size_t FlvReader::DecodeChunkInternal(const unsigned char* data, size_t size)
{
	BinaryReader reader(data, size);
	size_t parsedByte = 0;
	size_t allSize = size;
	while (allSize)
	{
		if (reader.Available() < 15)
		{
			break;
		}
		
		int lastTagSize = reader.Read32();
		unsigned char type = reader.Read8();
		unsigned int dataSize = reader.Read24();
		unsigned int time = reader.Read24() | (reader.Read8() << 24);
	//	printf("time is %d\n", time);
		unsigned char streamId = reader.Read24();
		
		if(reader.Available() < dataSize)
		{
			break;
		}
		
		switch (type)
		{
			case base::AMF_TYPE_VIDEO:
			{
				unsigned char codec = reader.Read8();
				unsigned char codecId = codec & 0x0f;
				unsigned char codecType = (codec >> 4) & 0x0f;
				if (codecType == 1)
				{
					a++;
					printf("key frame is %d\n",a);
				}

				if (codecId == Codec::FLV_CODEC_H264)  //H264 AVCVideoPacket
				{
					unsigned char nalutype = reader.Read8();
					unsigned int  ptsoffset = reader.Read24();
				//	printf("ptsoffset is %d\n", ptsoffset);
					MediaPacket packet;
			//		packet.frame = (FrameType)codecType;
				//	packet.timeoffset = time + ptsoffset;
					packet.SetTimeStamp(time + ptsoffset);
					if (nalutype == 0) //SPS/PPS
					{
			//			printf("sps/pps--\n");
					//	std::shared_ptr<Buffer>  sps(new Buffer(30));
					//	std::shared_ptr<Buffer>  pps(new Buffer(30));
						packet.SetFrameType(MediaPacket::VIDEO_CONFIG);
						std::string sps, pps;
						int ret = AVC::ReadVideoConfig(reader.Current(), reader.Available(), sps, pps);
						
							
							packet.buffer.reset(new DrainableBuffer(sps.size()));
							packet.buffer->Write(static_cast<const void *>(sps.c_str()), sps.size());
							readCallback_(packet);


							
							packet.buffer.reset(new DrainableBuffer(pps.size()));
							packet.buffer->Write(static_cast<const void *>(pps.c_str()), pps.size());
							readCallback_(packet);
						
					//	packets_.push_back();
					//	_buffers.push_back(sps);
					//	_buffers.push_back(pps);
						reader.Next(dataSize - 5);
					}
					else
					{
					//	printf("h264 video audio\n");
						unsigned int countsize = 0;
						while (countsize < dataSize - 5)
						{
							MediaPacket packet;
							packet.SetFrameType(MediaPacket::VIDEO);
						//	packet.frame = (FrameType)codecType;
					//		packet.timeoffset = time + ptsoffset;
						//	packet.buffer->Write(static_cast<const void *>(pps.c_str()), pps.size());
							unsigned int naluLength = reader.Read32();
						//	std::shared_ptr<Buffer> sendBuffer = std::make_shared<Buffer>(naluLength);
							packet.buffer.reset(new DrainableBuffer(naluLength));
					//		MediaPacket packet;
					//		packet.frame = 
						//	std::shared_ptr<Buffer> sendBuffer = std::make_shared<Buffer>(reader.Current(), naluLength);
							unsigned int len = reader.Read((unsigned char *)packet.buffer->GetWritePtr(), naluLength);
							packet.buffer->AddWritePos(len);
							
							readCallback_(packet);
					//		sendBuffer->setsize(len);
					//		_buffers.push_back(sendBuffer);
						//	sendBuffer.reset();
							countsize += (naluLength + 4);
						}
					}
				}
				else
				{
					reader.Next(dataSize - 8);
				}
				//_buffers.push_back(sendBuffer);
				//	source.writeVideo(track ? track : 1, _video, content);
				break;
			}
			case base::AMF_TYPE_AUDIO:
			{
				reader.Next(dataSize);
				break;
			}
			case base::AMF_TYPE_DATA:
			{
				reader.Next(dataSize);
				break;
			}
			default:
				break;
		}

		parsedByte += 15 + dataSize;
		allSize -= (15 + dataSize);	
	}
		
	return parsedByte;;		
}
		
}
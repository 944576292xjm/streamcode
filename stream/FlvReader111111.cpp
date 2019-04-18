#include "FlvReader.h"
#include "BinaryReader.h"
#include "Media.h"
#include "AVC.h"


namespace base {


FlvReader::FlvReader() : firstRead(true), _size(0), _type(AMF_TYPE_EMPTY)
{
}

FlvReader::~FlvReader()
{
}

int FlvReader::Decode(std::shared_ptr<Buffer> buffer)
{
	BinaryReader reader(buffer->data(), buffer->size());

	if (firstRead)  //先判断是否是flv文件格式
	{
		while (true)
		{
			if (reader.Available() < FLV_HEADER_LENGTH)
			{
				return reader.Available();
			}

			if (reader.Read8() == 0x46 && reader.Read8() == 0x4c && reader.Read8() == 0x56) //"FLV"
			{
				firstRead = false;
				reader.Next(2);
			//	reader.Next(6);
				reader.Next(reader.Read32() - 9);
				break;
			}
		}
	}
	while (reader.Available()){
		if (!_type)
		{
			if (reader.Available() < 5)
			{
				return reader.Available();
			}
			reader.Next(4);
			_type = (AMFType)reader.Read8();
		}

		if (!_size)
		{
			if (reader.Available() < 3)
			{
				return reader.Available();
			}
			_size = reader.Read24() + 7;
		}

		if (reader.Available() < _size)
		{
			return reader.Available();
		}

		switch (_type)
		{
		case base::AMF_TYPE_VIDEO:
		{
			//printf("video\n");
			unsigned int time = reader.Read24() | (reader.Read8() << 24);
			unsigned char track = reader.Read24();

			unsigned char codec = reader.Read8();
			unsigned char codecId = codec & 0x0f;
			unsigned char codecType = (codec >> 4) & 0x0f;

		//	std::shared_ptr<Buffer> sendBuffer;
		//	std::shared_ptr<Buffer> sendBuffer(new Buffer(const_cast<unsigned char*>(reader.Current()), _size - 8));
			if (codecId == Codec::FLV_CODEC_H264)  //H264 AVCVideoPacket
			{
				unsigned char nalutype = reader.Read8();
				unsigned int ptsoffset = reader.Read24();
				if (nalutype == 0) //SPS和PPS
				{
				//	std::shared_ptr<Buffer> config(new Buffer(1024));
					//sendBuffer->setsize(0);
					std::shared_ptr<Buffer>  sps(new Buffer(30));
					std::shared_ptr<Buffer>  pps(new Buffer(30));
					int ret = AVC::ReadVideoConfig(reader.Current(), reader.Available(), sps, pps);
					_buffers.push_back(sps);
					_buffers.push_back(pps);
					reader.Next(_size - 12);
				}
				else
				{
					unsigned int countsize = 0;
					while (countsize< _size - 12)
					{
						unsigned int naluLength = reader.Read32();
						std::shared_ptr<Buffer> sendBuffer = std::make_shared<Buffer>(naluLength);
					//	std::shared_ptr<Buffer> sendBuffer = std::make_shared<Buffer>(reader.Current(), naluLength);
						unsigned int len = reader.Read(sendBuffer->data(), naluLength);
						sendBuffer->setsize(len);
						_buffers.push_back(sendBuffer);
						sendBuffer.reset();
						countsize += (naluLength + 4);
					}
				}
				//reader.Next(_size - 12);
				//sendBuffer->Next(4);
			}
			else
			{
				reader.Next(_size - 8);
			}
			//_buffers.push_back(sendBuffer);
			//	source.writeVideo(track ? track : 1, _video, content);
			break;
		}
		case base::AMF_TYPE_AUDIO:
		{
		//	printf("video\n");
			unsigned int time = reader.Read24() | (reader.Read8() << 24);
			unsigned char track = reader.Read24();

			reader.Next(_size - 7);
			break;
		}
		case base::AMF_TYPE_DATA:
			reader.Next(_size);
			break;

		default:
			break;
		}

		_size = 0;
		_type = AMF_TYPE_EMPTY;
	}

	return 0;
	
}




}
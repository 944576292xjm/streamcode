#include "RTMP/RtmpSession.h"
#include "AMFReader.h"
#include "RtmpSource.h"
#include "Media.h"
#include "AVC.h"
#include "MediaPacket.h"
namespace base {


std::pair<std::string, RtmpSession::RtmpCmdFunction> pairArray[] =
{
	std::make_pair("connect", &RtmpSession::RtmpConnect),
	std::make_pair("_checkbw", &RtmpSession::RtmpCheckBw),
	std::make_pair("createStream", &RtmpSession::RtmpCreateStream),
	std::make_pair("play", &RtmpSession::RtmpPlay),
	std::make_pair("releaseStream", &RtmpSession::RtmpReleaseStream),
	std::make_pair("FCPublish", &RtmpSession::RtmpFCPublish),
	std::make_pair("publish", &RtmpSession::RtmpPublish),
	std::make_pair("FCSubscribe", &RtmpSession::RtmpFCSubscribe),
	//make_pair(2, 3),
	//make_pair(4, 5)
};
RtmpSession::RtmpCmdMap RtmpSession::g_mapCmd(pairArray, pairArray + sizeof(pairArray) / sizeof(pairArray[0]));

RtmpSession::RtmpSession(std::shared_ptr<TcpConnection> connection) : tcpconn(connection), _decoder(connection)
{

}

RtmpSession::~RtmpSession()
{
}

int RtmpSession::OnRecv(std::shared_ptr<TcpConnection> conn)
{
	int ret = 0;
	ret = _decoder.decode(tcpconn->inputBuffer_);
	//if (ret != 0)
	{
		//return buffer->size();
	}

	if (_decoder._packets.size())
	{
		//Signal();
		Run();
	}

	return ret;
}


uint64_t  RtmpSession::Run()
{
	for (auto _packet : _decoder._packets)
	{
		switch (_packet.type)
		{
			case AMF_TYPE_INVOCATION_AMF3:
			case AMF_TYPE_INVOCATION:
			{
				AMFReader amfReader((unsigned char *)(_packet.iobuf->GetReadPtr()), _packet.iobuf->Size());
				//AMFDecoder dec(chunkData.strBuf, chunkData.typeId == MSG_CMD3 ? 1 : 0);
				//onProcessCmd(dec);
				std::string method = amfReader.AMF_ReadString();

				printf("method is %s\n", method.c_str());
			//	double id = amfReader.AMF_ReadNumber();
				//m_dNowReqID = dec.load<double>();
				auto it = g_mapCmd.find(method);
				if (it == g_mapCmd.end())
				{
					//TraceL << "can not support cmd:" << method;
					break;
				}
				auto func = it->second;
				//(*func)(amfReader);
				(this->*func)(amfReader);
				break;
			}
			/*	case MSG_DATA:
				case MSG_DATA3: {
				AMFDecoder dec(chunkData.strBuf, chunkData.typeId == MSG_CMD3 ? 1 : 0);
				std::string type = dec.load<std::string>();
				TraceL << "notify:" << type;
				if (type == "@setDataFrame") {
				setMetaData(dec);
				}
				}
				break;*/
			case AMF_TYPE_AUDIO:
			{
				break;
			}
			case AMF_TYPE_VIDEO:
			{
#if 1
			BinaryReader reader((unsigned char *)(_packet.iobuf->GetReadPtr()), _packet.iobuf->Size());

				unsigned char codec(reader.Read8());
				unsigned char codecId = codec & 0x0f;
				unsigned char codecType = (codec >> 4) & 0x0f;
				//printf("codecType is %d\n", codecType);
				//_packet.frame = (Frame)codecType;

			//	unsigned char codecs = reader.Read8();
				MediaPacket packet;
				packet.SetChannelId(_packet.channelId);
				packet.SetStreamId(_packet.streamId);

				if (codecType == FLV_FRAME_KEY)
				{
					packet.SetKeyFrame();
				}
			//	packet.channelId = _packet.channelId;
		//		packet.streamId = _packet.streamId;
		//		packet.frame = (FrameType)codecType;
			//	packet.timeoffset = _packet.absoluteTime;
			//	printf("packet.timeoffset  is %x\n", packet.timeoffset);
				packet.buffer = std::move(_packet.iobuf);
				if (codecId == Codec::FLV_CODEC_H264)  //H264 AVCVideoPacket
				{
			//		packet.type = FLV_TAG_VIDEO;
					unsigned char nalutype = reader.Read8();
					unsigned int ptsoffset = reader.Read24();
					packet.SetTimeStamp(_packet.absoluteTime + ptsoffset);
					packet.SetFrameType(MediaPacket::VIDEO);
				//	printf("ptsoffset is %x\n", ptsoffset);
				//	packet.timeoffset = packet.timeoffset + ptsoffset;
				//	printf("time is %d\n", _packet.time);
				//	printf("timeoffset is %d\n", _packet.absoluteTime);
					//_packet->Next(5);
					if (nalutype == 0) //SPSºÍPPS
					{
					//	std::shared_ptr<Buffer>  sps(new Buffer());
					//	std::shared_ptr<Buffer>  pps(new Buffer());
					//	int readlen = AVC::ReadVideoConfig(reader.Current(), reader.Available(), sps, pps);
					//	packet.frame = FLV_FRAME_CONFIG;
					//	m_pPublisherSrc->SetVideoConfig(std::make_shared<RtmpChannel>(_packet));
						m_pPublisherSrc->PushPacketConfig(packet);
						//reader.Next(readlen);
					//	m_pPublisherSrc->writeVideo(sps);
					//	m_pPublisherSrc->writeVideo(pps);
						break;
					}
			//		packet.SetTimeStamp(_packet.absoluteTime + ptsoffset);
				//	packet.timeoffset = packet.timeoffset + ptsoffset;
				//	std::make_shared<RtmpChannel>(_packet);
					m_pPublisherSrc->PushPacket(packet);
				}
				/*if (!m_pPublisherSrc) {
					throw std::runtime_error("Not a rtmp publisher!");
					}
					m_pPublisherSrc->onGetMedia(std::make_shared<RtmpChannel>(chunkData));
					if (!m_bPublisherSrcRegisted && m_pPublisherSrc->ready()){
					m_bPublisherSrcRegisted = true;
					m_pPublisherSrc->regist();
					}*/
#endif
				break;
			}
		}
	}
	_decoder._packets.clear();
	return 0;
}

void RtmpSession::ReadFromSource(MediaPacket packet)
//void RtmpSession::ReadFromSource(std::shared_ptr<DrainableBuffer> buffer)
{
	_decoder.SendVideo(packet);
	//tcpconn->Send(packet.buffer->GetReadPtr(), packet.buffer->Size());
}

#if 0
int RtmpSession::WriteVideo(std::shared_ptr<Packet> packet)
{
	std::shared_ptr<Packet> RtmpChannel;
	if (_videoPackets.size())
	{
		_videoPackets.push_back(packet);
		RtmpChannel = _videoPackets.front();
		_videoPackets.pop_front();
	}
	else
	{
		RtmpChannel = packet;
	}

//	Frame type = packet->frame;
	//printf("type is %d\n", type);
//	_buffers.pop_front();
	//if (_videoPackets.front())
	return _decoder.SendVideo(RtmpChannel);
}
#endif

int RtmpSession::RtmpCheckBw(AMFReader &reader)
{
	double id = reader.AMF_ReadNumber();
	_decoder.SendBWResult(id);
	return 0;
}

int RtmpSession::RtmpReleaseStream(AMFReader &reader)
{
	double id = reader.AMF_ReadNumber();
	_decoder.SendFMLESmResult(id);
	return 0;
}

int RtmpSession::RtmpFCSubscribe(AMFReader &reader)
{
	double id = reader.AMF_ReadNumber();
	_decoder.SendFMLESmResult(id);
	return 0;
}

int RtmpSession::RtmpFCPublish(AMFReader &reader)
{
	double id = reader.AMF_ReadNumber();
	reader.AMF_ReadNull();
	std::string string = reader.AMF_ReadString();
	_decoder.SendFMLESmResult(id);
	//_decoder.SendOnFcPublish(id);
	return 0;
}

int RtmpSession::RtmpPublish(AMFReader &reader)
{
	bool ok = false;
	double id = reader.AMF_ReadNumber();
	reader.AMF_ReadNull();
	std::string stream = reader.AMF_ReadString();
	std::string type = reader.AMF_ReadString();

	auto src = RtmpSource::FindMediaSource(app);
	if (!src)
	{
		ok = false;
	}

	m_pPublisherSrc = (RtmpSource::CreateMediaSource(app));
	
	
	//m_pPublisherSrc = std::make_shared<RtmpSource>(*_src);
	//m_pPublisherSrc.reset(_src);

	_decoder.SendPubOnStatus(id, true);
	return 0;
}

int RtmpSession::RtmpCreateStream(AMFReader &reader)
{
	double id = reader.AMF_ReadNumber();
	_decoder.SendcCreateStream(id);
	return 0;
}

int RtmpSession::RtmpPlay(AMFReader &reader)
{
	bool stauts = true;;
	double id = reader.AMF_ReadNumber();
	reader.AMF_ReadNull();
	std::string stream = reader.AMF_ReadString();

	std::string source = !app.empty() ? app : stream;
	auto src = RtmpSource::FindMediaSource(source);
	if (!src)
	{
		stauts = false;
	}
//	src->AddConsumer(this);
	//_videoConfig = src->GetVideoConfig();
//	src->GetGopCache(_videoPackets);

	_decoder.SendStreamBegin0();
	_decoder.SendStatusReset(true);
	_decoder.SendStatusStart();
	_decoder.RtmpSampleAccess();

	std::weak_ptr<RtmpSession> wksession = shared_from_this();
	//	src->SetReadCallback(std::bind(&RtmpSession::ReadFromSource, 
	//	std::weak_ptr<RtmpSession>(shared_from_this()), std::placeholders::_1));
	src->StartRead([wksession](MediaPacket packet) {
		std::shared_ptr<RtmpSession> session = wksession.lock();
		if (session)
		{
			session->ReadFromSource(packet);
		}
	});
//	_decoder.SendVideo(src->GetVideoConfig());
	printf("play complete------------\n");
	return 0;
}

int RtmpSession::RtmpConnect(AMFReader &reader)
{
	//std::string name = reader.AMF_ReadString();

	double id = reader.AMF_ReadNumber();

	AMFValue object = reader.AMF_ReadObject();
	AMFValue appvalue = object["app"];
	app = appvalue.GetString();

	AMFValue urlvalue = object["tcUrl"];
	std::string url = urlvalue.GetString();
	AMFValue audiocodecsvalue = object["audioCodecs"];
	if (audiocodecsvalue.type() == AMFValue::AMFVALUE_NUMBER)
	{
		//double audioCodecs = audiocodecsvalue.GetNumber();
	}

	_decoder.SendWinAckSize(WIN_ACK_SIZE_DEFAULT);
	_decoder.SendPeerWidth(PEERWIDTH_DEFAULT);
	//	 value = object["objectEncoding"];
	//	unsigned int objectEncoding = value.value<unsigned int>();*/
	bool ok = true;
	
	_decoder.SendConnectStatus(ok,id);
	//_decoder.SendOnBWDone();

	return 0;
}

	
}
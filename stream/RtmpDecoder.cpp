#include "RTMP/RtmpDecoder.h"
#include "BinaryReader.h"
#include "AMF.h"
#include "AMFReader.h"
#include "AMFWriter.h"
#include "Buffer.h"

namespace base {

RtmpDecoder::RtmpDecoder(std::shared_ptr<TcpConnection> conn) : conn_(conn), _state(HANDSHAKE_C0C1), _chunkSize(RTMP_DEFAULT_CHUNKSIZE), _localchunkSize(RTMP_DEFAULT_CHUNKSIZE)
{
}

RtmpDecoder::~RtmpDecoder()
{
}

int RtmpDecoder::SendResponse()
{
	return 0;
}

int RtmpDecoder::SendRtmpProtol(AMFType type, unsigned int chunkid, unsigned int streamid, const char *data, unsigned int datasize, unsigned int time)
{
	RtmpHeader header;
	header.flags = (chunkid & 0x3f);
	header.typeId = type;
	BinaryReader::Flip24((void *)header.timeStamp, time);
	BinaryReader::Flip24((void *)header.bodySize, datasize);
	memcpy(header.streamId, &streamid, 4);
	//set_be24(header.timeStamp, bExtStamp ? 0xFFFFFF : ui32TimeStamp);
	//set_be24(header.bodySize, strBuf.size());
	//set_le32(header.streamId, ui32StreamId);
	Buffer buffer;
	
	{
		//unsigned int size = data.size();
		//bool first = true;
		unsigned int pos = 0;
		while (pos < datasize)
		{
			if (!pos)
			{
				buffer.Append(&header, sizeof(header));
			}
			else
			{
				unsigned char flags = (chunkid & 0x3f) | (3 << 6);
				buffer.Append(&flags, 1);
			}
			unsigned int chunk = min(_localchunkSize, datasize - pos);
		//	memcpy(buffer->data() + totalSize, strBuf.data() + pos, chunk);
			buffer.Append(data + pos, chunk);
			pos += chunk;
		}
	}
	
	conn_->Send((void *)buffer.data(), buffer.size());
	return 0;
}

int RtmpDecoder::SendAgreement(AMFType type, std::string data)
{
	SendRtmpProtol(type, CHUNK_AGREEMENT_CHUNKID, CONTROL_STREAM_ID, data.c_str(), data.size());
	return 0;
}

int RtmpDecoder::SendChunkSize(unsigned int chunksize)
{
	unsigned int size = htonl(chunksize);
	std::string set_chunk((char *)&size, 4);
	SendAgreement(AMF_TYPE_CHUNKSIZE, set_chunk);
	//m_iChunkLenOut = ui32Size;
	return 0;
}

int RtmpDecoder::SendWinAckSize(unsigned int acksize)
{
	unsigned int size = htonl(acksize);
	std::string set_chunk((char *)&size, 4);
	SendAgreement(AMF_TYPE_WIN_ACKSIZE, set_chunk);
	//m_iChunkLenOut = ui32Size;
	return 0;
}

int RtmpDecoder::SendPeerWidth(unsigned int peerwidth)
{
	std::string peerBandwidth((char *)&peerwidth, 4);
	//peerBandwidth.push_back((char)0x02);
	peerBandwidth.append(0x02,1);
	SendAgreement(AMF_TYPE_BANDWIDTH, peerBandwidth);
	return 0;
}

int RtmpDecoder::SendOnBWDone()
{
	AMFWriter writer;
	writer.AMF_WriteString("onBWDone");
	writer.AMF_WriteNumber(0);
	writer.AMF_WriteNull();
	SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendResult(bool ok, double id, AMFValue object, AMFValue value)
{
	AMFWriter writer;
	writer.AMF_WriteString("_result");
	writer.AMF_WriteNumber(id);
	writer.AMFWriteValue(object);
	writer.AMFWriteValue(value);
	//writer.AMF_WriteNull();
	//writer.AMF_WriteNumber(1);
	SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendBWResult(double id)
{
	AMFWriter writer;
	AMFValue object(AMFValue::AMFVALUE_NULL);
	AMFValue value(AMFValue::AMFVALUE_NULL);
	//writer.AMF_WriteString("_result");
	//writer.AMF_WriteNumber(id);
	//writer.AMF_WriteNull();
	//writer.AMF_WriteNumber(1);
	SendResult(true, id, object, value);
	//SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
	//writer.AMF_WriteNull();
	//writer.AMF_WriteNumber(1);
	//SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendcCreateStream(double id)
{
	//AMFWriter writer;
//	AMFValue object(AMFValue::AMFVALUE_NULL);
//	AMFValue streamid((double)1);
	//writer.AMF_WriteString("_result");
	//writer.AMF_WriteNumber(id);
	//writer.AMF_WriteNull();
	//writer.AMF_WriteNumber(1);
	//SendResult(true, id, object, streamid);

	AMFWriter writer;
	writer.AMF_WriteString(RTMP_AMF0_COMMAND_RESULT);
	writer.AMF_WriteNumber(id);
	writer.AMF_WriteNull();
	SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
	//SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendPubOnStatus(double id, bool ok)
{
	AMFWriter writer;
	AMFValue status(AMFValue::AMFVALUE_OBJECT);
	status.add("level", ok ? "status" : "error");
	status.add("code", ok ? "NetStream.Publish.Start" : "NetStream.Publish.BadName");
	status.add("description", ok ? "Started publishing stream." : "Already publishing.");
	status.add("clientid", "001");
	writer.AMF_WriteString(RTMP_AMF0_COMMAND_ON_STATUS);
	writer.AMF_WriteNumber(id);
	writer.AMF_WriteNull();
	writer.AMF_WriteObject(status);
	SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM2, 0, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendFMLESmResult(double id)
{
	AMFWriter writer;
	AMFValue object(AMFValue::AMFVALUE_NULL);
	AMFValue value(AMFValue::AMFVALUE_UNDEFINED);
	//writer.AMF_WriteString("_result");
	//writer.AMF_WriteNumber(id);
	//writer.AMF_WriteNull();
	//writer.AMF_WriteNumber(1);
	SendResult(true, id, object, value);
	//SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendOnFcPublish(double id)
{
	AMFWriter writer;
	AMFValue status(AMFValue::AMFVALUE_OBJECT);
	status.add("code","NetStream.Publish.Start");
	status.add("description","Started publishing stream.");
	writer.AMF_WriteString(RTMP_AMF0_COMMAND_ON_FC_PUBLISH);
	writer.AMF_WriteNumber(0);
	writer.AMF_WriteNull();
	writer.AMF_WriteObject(status);
	SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM2, 0, writer.Data(), writer.Size());
	return 0;
}

int  RtmpDecoder::SendStatus(AMFValue value)
{
	AMFWriter writer;
	writer.AMF_WriteString("onStatus");
	writer.AMF_WriteNumber(0);
	writer.AMF_WriteNull();
	writer.AMF_WriteObject(value);
	SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM2, 0, writer.Data(), writer.Size());
	//sendReply("onStatus", nullptr, status);
	return 0;
}

int RtmpDecoder::SendStatusReset(bool success)
{
	AMFValue status(AMFValue::AMFVALUE_OBJECT);
	status.add("level", success ? "status" : "error");
	status.add("code", success ? "NetStream.Play.Reset" : "NetStream.Play.StreamNotFound");
	status.add("description", success ? "Playing and resetting stream." : "No such stream.");
	status.add("details", "stream");
	status.add("clientid", "ASAICiss");
	SendStatus(status);
	return 0;
}

int  RtmpDecoder::SendStatusStart()
{
	AMFValue status(AMFValue::AMFVALUE_OBJECT);
	status.add("level", "status");
	status.add("code", "NetStream.Play.Start");
	status.add("description", "Started playing stream.");
	status.add("details", "stream");
	status.add("clientid", "ASAICiss");
	SendStatus(status);
	return 0;
}

int  RtmpDecoder::RtmpSampleAccess()
{
	AMFWriter writer;
	writer.AMF_WriteString(RTMP_AMF0_DATA_SAMPLE_ACCESS);
	writer.AMF_WriteBoolean(true);
	writer.AMF_WriteBoolean(true);
	SendRtmpProtol(AMF_TYPE_RAW, CHUNK_AGREEMENT_CHUNKID, 1, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendStreamBegin0() 
{
	AMFWriter writer;
	writer.AMF_Write16(CONTROL_STREAM_BEGIN);
	writer.AMF_Write32(0);
	SendRtmpProtol(AMF_TYPE_RAW, CHUNK_AGREEMENT_CHUNKID, CONTROL_STREAM_ID, writer.Data(), writer.Size());
	return 0;
}

int RtmpDecoder::SendConnectStatus(bool result, double id)
{
	/*AMFWriter writer;
	
	AMFValue version(AMFValue::AMFVALUE_OBJECT);

	version.add("fmsVer", "FMS/RTMP_SIG_FMS_VER");
	version.add("capabilities", 127);
	AMFValue status(AMFValue::AMFVALUE_OBJECT);
	status.add("level",result ? "status" : "error");
	status.add("code", result ? "NetConnection.Connect.Success" : "NetConnection.Connect.InvalidApp");
	status.add("description", result ? "Connection succeeded." : "InvalidApp.");
	status.add("objectEncoding", RTMP_SIG_AMF0_VER);

	writer.AMF_WriteString(result ? "_result" : "_error");
	writer.AMF_WriteNumber(id);
	writer.AMF_WriteObject(version);
	writer.AMF_WriteObject(status);*/

	AMFWriter writer;

	AMFValue version(AMFValue::AMFVALUE_OBJECT);
	version.add("fmsVer", "FMS/3,0,1,123");
	version.add("capabilities", 31.0);
	AMFValue status(AMFValue::AMFVALUE_OBJECT);
	status.add("level", result ? "status" : "error");
	status.add("code", result ? "NetConnection.Connect.Success" : "NetConnection.Connect.InvalidApp");
	status.add("description", result ? "Connection succeeded." : "InvalidApp.");
	status.add("objectEncoding", 0.0);

	writer.AMF_WriteString(result ? "_result" : "_error");
	writer.AMF_WriteNumber(id);
	writer.AMF_WriteObject(version);
	writer.AMF_WriteObject(status);


	SendRtmpProtol(AMF_TYPE_INVOCATION, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());

	return 0;
}

int RtmpDecoder::SendVideo(MediaPacket packet)
{
	//RtmpChannel *RtmpChannel = dynamic_cast<RtmpChannel *>(packet.get());
//	RtmpChannel *RtmpChannel = (RtmpChannel*)(packet.get());
	//std::shared_ptr<RtmpChannel> RtmpChannel = static_cast<std::shared_ptr<RtmpChannel>>(packet);
	//return SendRtmpProtol(AMF_TYPE_VIDEO, CHUNK_CLIENT_CONTROL_STREAM1, 0, writer.Data(), writer.Size());
//	printf("packet.timeoffset is %x\n", packet.timeoffset);
	return SendRtmpProtol(AMF_TYPE_VIDEO, packet.GetChannelId(), packet.GetStreamId(), packet.buffer->GetReadPtr(),packet.buffer->Size(), packet.GetTimeStamp());
}

int RtmpDecoder::decode(RingBuffer& buffer)
{
	while (buffer.Size())
	{
		if (_state == HANDSHAKE_C0C1)
		{
			if (buffer.Size() < 1537) //等到C0C1一起到
				return buffer.Size();

			if (buffer.Size() > 1537) {
				//ERROR("RTMP Handshake unknown");
				//socket.shutdown();
				return 0;
			}

		//	buffer.AddReadPos(1537);

		//	unsigned char type(*buffer->data());
			unsigned char type(buffer.Read8());
			if (type != RTMP_VERSION)
			{

				return 0;
			}

			//发送S0
			char handshake_c0 = RTMP_VERSION;
			conn_->Send(&handshake_c0, HANDSHAKE_C0_LEN);
			//发送S1
			RtmpHandshake s1(0);
			conn_->Send((void *)&s1, HANDSHAKE_C1_LEN);

			char c2[HANDSHAKE_C2_LEN];
			buffer.Read(static_cast<void *>(c2), HANDSHAKE_C2_LEN);
			//发送S2
			conn_->Send(static_cast<void *>(c2), HANDSHAKE_C2_LEN);

			//等待C2
			_state = HANDSHAKE_C2;

			return 0;
		}
		else if (_state == HANDSHAKE_C2)
		{
			if (buffer.Size() < HANDSHAKE_C2_LEN) //等到C0C1一起到
				return 0;
			buffer.AddReadPos(HANDSHAKE_C2_LEN);
			_state = HANDSHAKE_DONE;
			//m_strRcvBuf.erase(0, C1_HANDSHARK_SIZE);
			//握手结束，进入命令模式
			//	if (!m_strRcvBuf.empty()) {
			//	handle_rtmp();
			//	}
			continue;
		}



		//BinaryReader reader(buffer->data(), buffer->size());

		bool isTimeRelative(true);
		unsigned char head_type = buffer.Peek8();
		unsigned char headLen = 12 - (head_type >> 6) * 4;
		if (!headLen)
		{
			headLen = 1;
		}
		unsigned char channelId = head_type & 0x3f;	
		unsigned char len(headLen);

		if (channelId < 2)
		{
			headLen += channelId + 1;
		}
		
	//	if (reader.Available() < len)
		if(buffer.Size() < len)
		{
		//	return buffer->Size();
			return 0;
		}

		if (channelId < 2)
		{
			int s = 0;
			//channelId = (channelId == 0 ? reader.Read8() : reader.Read16()) + 64;
		}

		RtmpChannel& channel_ = channels_[channelId];
		channel_.channelId = channelId;

		if (headLen >= 4)
		{
			channel_.time = RingBuffer::Flip24(buffer.Peek24(1));

			if (headLen >= 8) 
			{			
				// SIZE
				//unsigned int bodySize = reader.Read24();
				channel_.bodySize = RingBuffer::Flip24(buffer.Peek24(4));
				// TYPE
				//unsigned char type = AMF::Type(reader.Read8());
				channel_.type = AMFType(buffer.Peek8(7));
				if (headLen >= 12) {
					isTimeRelative = false;
					// STREAM
				//	channel_.streamId = buffer.Peek8();
				//	channel_.streamId += buffer.Peek8() << 8;
				//	channel_.streamId += buffer.Peek8() << 16;
				//	channel_.streamId += buffer.Peek8() << 24;
				//	printf("channel id is %d\n", channel_.streamId);
					channel_.streamId = buffer.Peek32(8);
						printf("channel id is %d\n", channel_.streamId);
					//packet.streamId = reader.Read32();
				}
			}
		}

		unsigned int chunkSize(channel_.bodySize);


		if (!channel_.iobuf)
		{
			// New packet
			channel_.iobuf.reset(new DrainableBuffer(chunkSize));
			if (isTimeRelative)
				channel_.absoluteTime += channel_.time; // relative
			else
				channel_.absoluteTime = channel_.time; // absolute

	//		printf("absoluteTime is %x\n", channel_.absoluteTime);
		}
		else if (channel_.iobuf->Size() > chunkSize)
		{
			//ERROR("Invalid RTMP packet, chunked message doesn't match bodySize");
			//	socket.shutdown();
			return 0;
		}
		else
		{
			chunkSize -= channel_.iobuf->Size();
		}

	/*	auto iMore = min(_iChunkLenIn, chunkData.bodySize - chunkData.strBuf.size());
		if (_strRcvBuf.size() < iHeaderLen + iOffset + iMore) {
			//need more data
			return;
		}*/


		chunkSize = min(chunkSize, _chunkSize);
	

		if (buffer.Size() < chunkSize + headLen)
		{
			return 0;
		}

		buffer.AddReadPos(headLen);
		buffer.Read(channel_.iobuf->GetWritePtr(), chunkSize);
		channel_.iobuf->AddWritePos(chunkSize);

	

	//	packet->Append(reader.Current(), chunkSize);
	//	chunkSize += len; // + headerSize!

	//	buffer->Next(chunkSize);

 		if (channel_.bodySize == channel_.iobuf->Size())
		{
			switch (channel_.type)
			{
				case AMF_TYPE_CHUNKSIZE:
				//	_chunkSize = BinaryReader(packet->data(), packet->size()).Read32();
				//	_chunkSize = channel_.iobuf->Peek32();
					_chunkSize = RingBuffer::Flip32(channel_.iobuf->Peek32());
					channel_.iobuf.reset();
					break;
				case AMF_TYPE_WIN_ACKSIZE:
					//_winAckSize = BinaryReader(packet.data(), packet.size()).read32();
					channel_.iobuf.reset();
					break;
				case AMF_TYPE_ACK:
					channel_.iobuf.reset();
					break;
			//	case AMF_TYPE_INVOCATION_AMF3:
			//	case AMF_TYPE_INVOCATION: 
				default:
				{
					_packets.push_back(std::move(channel_));
					break;
				}
			//	default:
			//	{
					
			//	}
			}
		}

	}
	return 0;
}




}
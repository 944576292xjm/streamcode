#pragma once
#if 1

#include "RTP.h"
#include "WinSock2.h"



namespace base {

	static unsigned int timestamp = 0;
	static unsigned int seq = 0;
	unsigned int RTP::SendNaluPacket(const unsigned char* sliceData, int sliceSize, unsigned  long  pts, TcpConnectionPtr tcpconn)
	{

		unsigned char tmpbuffer[1460];

		tmpbuffer[0] = '$';
		tmpbuffer[1] = 0;


		RTP_FIXED_HEADER *rtp_hdr = (RTP_FIXED_HEADER *)(tmpbuffer + 4);
		rtp_hdr->extension = 0;
		rtp_hdr->padding = 0;
		rtp_hdr->csrc_len = 0;
		rtp_hdr->payload = 96;  //负载类型号  
		rtp_hdr->version = 2;  //版本号，此版本固定为2  
		rtp_hdr->ssrc = htonl(10);    //随机指定为10，并且在本RTP会话中全局唯一  
		rtp_hdr->marker = 0;
	//	rtp_hdr->seq_no = htons(seq++);
		//rtp_hdr->timestamp = htonl(pts);
		printf("pts is %d\n", pts);
		rtp_hdr->timestamp = htonl(pts * 90);
		//rtp_hdr->timestamp = htonl(timestamp += 40);

		unsigned char *buffer = tmpbuffer + sizeof(RTP_FIXED_HEADER) + 4;


		// NALU 小于最大 RTP 包大小的情况 
		if (sliceSize < 1350) {


	//		printf("type is %d\n", sliceData[0] & 0x1f);
			rtp_hdr->seq_no = htons(seq++);
			rtp_hdr->marker = 1;
			memcpy(buffer, sliceData, sliceSize);
			tmpbuffer[2] = ((sliceSize + sizeof(RTP_FIXED_HEADER)) & 0xFF00) >> 8;
			tmpbuffer[3] = ((sliceSize + sizeof(RTP_FIXED_HEADER)) & 0x00FF);
	//		int ret = videoSocket->Send((const char *)tmpbuffer, sliceSize + sizeof(RTP_FIXED_HEADER) + 4);
			tcpconn->Send(static_cast<void *>(tmpbuffer), sliceSize + sizeof(RTP_FIXED_HEADER) + 4);
			//printf("ret is %d\n", ret);
			return 0;
			//return SendPacket(sliceData, sliceSize, isEnd, TRUE, type, pts, timestamp);
		}

		// 如果一个 NALU 大于最大的 RTP 包的大小, 则需要把它进行分片后打包发送 

		unsigned char nalHeader = sliceData[0]; // NALU 头 
		const unsigned char* data = sliceData + 1;
		int leftover = sliceSize - 1;
		bool isStart = true;
		bool isEnd;

	//	printf("type is %d\n", nalHeader & 0x1f);
		while (leftover > 0) {
			int size = min(1350, leftover);
			isEnd = (size == leftover);

			int t = nalHeader;
			


			// 构建 FU 头 
			buffer[0] = (nalHeader & 0x60) | 28;	// FU indicator 
			buffer[1] = (nalHeader & 0x1f);	// FU header 
			if (isStart) {
				rtp_hdr->marker = 0;
				buffer[1] |= 0x80;
			}
			if (isEnd) {
				rtp_hdr->marker = 1;
				buffer[1] |= 0x40;
			}

			memcpy(buffer + 2, data, size);
			//SendPacket(buffer, size + 2, isEnd, TRUE, type, pts, timestamp);
			tmpbuffer[2] = ((size + 2 + sizeof(RTP_FIXED_HEADER)) & 0xFF00) >> 8;
			tmpbuffer[3] = ((size + 2 + sizeof(RTP_FIXED_HEADER)) & 0x00FF);
			rtp_hdr->seq_no = htons(seq++);
		//	int ret = videoSocket->Send((const char *)tmpbuffer, size + 2 + sizeof(RTP_FIXED_HEADER) + 4);
			tcpconn->Send(static_cast<void *>(tmpbuffer), size + 2 + sizeof(RTP_FIXED_HEADER) + 4);
		//	printf("ret is %d\n", ret);
			leftover -= size;
			data += size;
			isStart = FALSE;
		}
		return 0;
	}
}



#endif



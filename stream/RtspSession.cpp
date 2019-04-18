#include "RtspSession.h"
#include <string.h>
#include "RtspRequest.h"
#include "RtspResponse.h"
//#include "base64.h"


namespace base {

RtspSession::RtspSession(TcpConnectionPtr conn) : tcpconn(conn)
{

}

RtspSession::~RtspSession()
{
}

void RtspSession::OnRead(TcpConnectionPtr conn)
{
	int ret = 0;
	ret = rtspRequest.Parse(conn->inputBuffer_);
	if (!ret)
	{
		return;
	}

	std::string method = rtspRequest.GetMethod();

	if (method.compare("OPTIONS") == 0)
	{
		ret = ProcessOption();
	}
	else if (method.compare("DESCRIBE") == 0)
	{
		ret = ProcessDescribe();
	}
	else if (method.compare("SETUP") == 0)
	{
		ret = ProcessSetup();
	}
	else if (method.compare("PLAY") == 0)
	{
		ret = ProcessPlay();
	}
	if (ret < 0)
	{
		//return;
	}
}

int RtspSession::ProcessOption()
{
	RtspResponse response;
	response.SetStatus(200);
	response.AddHeader("CSeq",rtspRequest.GetValue("CSeq"));
	response.AddHeader("Public", "OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, \
		PAUSE, SET_PARAMETER, GET_PARAMETER");
	std::string sendpacket = response.BuildHeader();
	tcpconn->Send(sendpacket);
#if 0
	char m_pcBuf[4 * 1024];
	int n = sprintf(m_pcBuf, "RTSP/1.0 200 OK\r\n"
		"CSeq: %s\r\n"
		//"Server: %s-%0.2f(build in %s)\r\n"
		//"%s"
		"Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY,"
		" PAUSE, SET_PARAMETER, GET_PARAMETER\r\n\r\n",
		RtspRequest["CSeq"].c_str());// , //g_serverName.data(),
		//	RTSP_VERSION, RTSP_BUILDTIME,
		//	dateHeader().data());
		socket->Send(m_pcBuf,n);
		printf("send is %s\n", m_pcBuf);
#endif
		return 0;
}

std::string sps;
std::string pps;


std::string CreateSdp()
{
	std::string m_strSDP;
	m_strSDP = "v=0\r\n";
	//m_strSDP += "o=- 1383190487994921 1 IN IP4 192.168.0.101\r\n";
	m_strSDP += "o=- 58 1 IN IP4 192.168.0.102\r\n";
	m_strSDP += "s=TinyStreamServer\r\n";
	m_strSDP += "i=HeheServer\r\n";
	m_strSDP += "c=IN IP4 192.168.0.102\r\n";
	m_strSDP += "t=0 0\r\n";
	m_strSDP += "a=range:npt=0-\r\n";
	//m_strSDP += "a=range:npt=0-\r\n";
	//直播，时间长度永远
	/*if (fDuration <= 0 || bLiveStream){
		m_strSDP += "a=range:npt=0-\r\n";
	}
	else{
		m_strSDP += StrPrinter << "a=range:npt=0-" << fDuration << "\r\n" << endl;
	}*/
	m_strSDP += "a=control:*\r\n";

	m_strSDP += "m=video 0 RTP/AVP 96\r\n";
	m_strSDP += "a=control:trackID=1\r\n";
	m_strSDP += "a=rtpmap:96 H264/90000\r\n";
/*	m_strSDP += "a=fmtp:96 packetization-mode=1;profile-level-id=";
//	m_strSDP += "b=AS:5100\r\n";

	char acTmp[256];

	int profile_level_id = (sps[1] << 16) | (sps[2] << 8) | sps[3]; //

	memset(acTmp, 0, sizeof(acTmp));
	sprintf(acTmp, "%06X", profile_level_id);
	m_strSDP += acTmp; 
	memset(acTmp, 0, sizeof(acTmp));
	av_base64_encode(acTmp, sizeof(acTmp), (unsigned char_t *)sps.c_str(), sps.size());
	//WarnL<<"SPS base64:"<<strTemp;
	//WarnL<<"SPS hexdump:"<<hexdump(SPS_BUF, SPS_LEN);
	m_strSDP += acTmp;
	m_strSDP += ",";
	memset(acTmp, 0, sizeof(acTmp));
	av_base64_encode(acTmp, sizeof(acTmp), (unsigned char_t *)pps.c_str(), pps.size());
	m_strSDP += acTmp;
	m_strSDP += "\r\n";*/
	return m_strSDP;
}

void RtspSession::ReadFromSource(MediaPacket packet)
{
	//printf("pts is %d\n", packet.timeoffset);
	RTP::SendNaluPacket((unsigned char *)(packet.buffer->GetReadPtr()), packet.buffer->Size(), packet.GetTimeStamp(), tcpconn);
}

int RtspSession::ProcessDescribe()
{
#if 1
	//if (RtspRequest["Accept"].c_str() && !strstr(RtspRequest["Accept"].c_str(), "application/sdp"))
	if (0)
	{
		//_writer.close(501); //如果请求不是SDP，返回不支持
	}
	else
	{

		/*auto src = RtspSource::FindMediaSource(app);
		if (!src)
		{
			stauts = false;
		}
		std::weak_ptr<RtmpSession> wksession = shared_from_this();
		//	src->SetReadCallback(std::bind(&RtmpSession::ReadFromSource, 
		//	std::weak_ptr<RtmpSession>(shared_from_this()), std::placeholders::_1));
		src->SetReadCallback([wksession](MediaPacket packet) {
			std::shared_ptr<RtmpSession> session = wksession.lock();
			if (session)
			{
				session->ReadFromSource(packet);
			}
		});*/

		//std::weak_ptr<RtspSource> _source = RtspSource::FindMediaSource("1.flv");
	/*	auto _source = RtspSource::FindMediaSource("1.flv");
		if (_source == NULL)
		{
			_source = RtspSource::CreateMediaSource("1.flv");
			
		//	file->Signal();
		}*/

		flvReader = std::make_shared<FlvReader>("E:\\xiqu.flv");
		flvReader->SetReadCallback(std::bind(&RtspSession::ReadFromSource, this, std::placeholders::_1));
		
		/*while (0)
		{
			if (!_source->sps.empty() && !_source->pps.empty())
			{
				break;
			}
			Sleep(100);
		}*/
	//	while (_source->sps.empty() && !_source->pps.empty());

	//	sps = _source->sps;
	//	pps = _source->pps;

		std::string sdp = CreateSdp();
	
		//std::string describe;
		//describe += ""
		char m_pcBuf[4*1024];
		int n = sprintf(m_pcBuf, "RTSP/1.0 200 OK\r\n"
			"CSeq: %s\r\n"
			//	"Server: %s-%0.2f(build in %s)\r\n"
			//	"%s"
			//	"x-Accept-Retransmit: our-retransmit\r\n"
				//"x-Accept-Dynamic-Rate: 1\r\n"
			"Content-Base: %s/\r\n"
			"Content-Type: application/sdp\r\n"
			"Content-Length: %d\r\n\r\n%s",
			rtspRequest.GetValue("CSeq").c_str(),
			//RtspRequest["CSeq"].c_str(),// g_serverName.data(),
			//RTSP_VERSION, RTSP_BUILDTIME,
			//dateHeader().data(),
		//	RtspRequest.GetUrl().c_str(),
		rtspRequest.GetUrl().c_str(),
			sdp.size(), sdp.c_str());

		tcpconn->Send(m_pcBuf, n);
//		printf("send size is %d, %s\n", t,m_pcBuf);
	}
#endif
	return 0;
}

int	RtspSession::ProcessSetup()
{
#if 1
//Transport: RTP / AVP; unicast; client_port = 60094 - 60095

	//RtspSource* source = RtspSource::FindMediaSource("1.flv");
	//if (source == NULL)
//	{
		//source = MediaSource::CreateMediaSource("1.FLV");
//	}

	int theTrackID = rtspRequest.GetTrackId();
	if (theTrackID < 0)
	{

	}

	std::string transport = rtspRequest.GetValue("Transport");
//	const char* clientPort = NULL;
//	unsigned int rtpPort = 0, rtcpPort = 0;
//	unsigned int srtpPort = 0, srtcpPort = 0;
	if (transport.find("RTP/AVP/TCP") != std::string::npos)
	{
		char m_pcBuf[4 * 1024];
		memset(m_pcBuf, 0, sizeof(4096));
		int n = sprintf(m_pcBuf, "RTSP/1.0 200 OK\r\n"
			"CSeq: %s\r\n"
			"Transport: RTP/AVP/TCP;unicast;interleaved=0-1\r\n"
			//	"client_port=%d-%d;server_port=%d-%d;ssrc=%I64d\r\n"
			"Session: %I64d\r\n\r\n",
			rtspRequest.GetValue("CSeq").c_str(),
			//rtpPort, rtcpPort, srtpPort, srtcpPort,

			TimeTick::GetMonotonicNow());
		tcpconn->Send(m_pcBuf, n);
	}


	/*if (!transport || !(clientPort = strstr(transport, "client_port="))) {
		return -1;
	}
	else
	{
		unsigned int rtpPort = 0, rtcpPort = 0;
		unsigned int srtpPort = 5557, srtcpPort = 5558;
		sscanf(clientPort, "client_port=%d-%d", &rtpPort, &rtcpPort);
		
		_consumer = _source->CreateConsumer();
		_consumer->InitVideo(GetPeerIp().c_str(), rtpPort, rtcpPort);

		char m_pcBuf[4 * 1024];
		memset(m_pcBuf, 0, sizeof(4096));
		int n = sprintf(m_pcBuf, "RTSP/1.0 200 OK\r\n"
			"CSeq: %s\r\n"
			"Transport: RTP/AVP;unicast;"
			"client_port=%d-%d;server_port=%d-%d;ssrc=%I64d\r\n"
			"Session: %I64d\r\n\r\n",
			RtspRequest["CSeq"].c_str(), rtpPort, rtcpPort, srtpPort, srtcpPort,

			Time::Now(), Time::Now()+2);

		
		printf("send is %s\n", m_pcBuf);
		socket->Send(m_pcBuf, n);
	}*/
#endif
	return 0;
}

int	RtspSession::ProcessPlay()
{
#if 1
	char m_pcBuf[4 * 1024];
	int n = sprintf(m_pcBuf, "RTSP/1.0 200 OK\r\n"
		"CSeq: %s\r\n"
		"Session: %s\r\n"
		"RTP-Info: %strack1\r\n\r\n",
		//"Range: npt=%.2f-\r\n"
		rtspRequest.GetValue("CSeq").c_str(),
		rtspRequest.GetValue("Session").c_str(),
		rtspRequest.GetUrl().c_str());
	tcpconn->Send(m_pcBuf, n);
	printf("send is %s\n", m_pcBuf);

	if (flvReader->Open() < 0)
	{
		return -1;
	}

	

#endif
	return 0;

}



}
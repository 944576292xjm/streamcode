#pragma once

#include <memory>
#include "Buffer.h"
#include "base/TimeTick.h"
#include "base/Callback.h"
#include "RtspRequest.h"
#include "base/TcpConnection.h"
#include "FlvReader.h"
#include "RtspSource.h"
#include "RTP.h"

namespace base {



enum CommandType 
{
	COMMAND_UNKNOWN = 0,
	COMMAND_OPTIONS,
	COMMAND_DESCRIBE,
	COMMAND_SETUP,
	COMMAND_PLAY,
	COMMAND_GET_PARAMETER,
	COMMAND_SET_PARAMETER,
	COMMAND_TEARDOWN,
};

class RtspSession 
{
public:
	RtspSession(TcpConnectionPtr conn);
	~RtspSession();

	void OnRead(TcpConnectionPtr conn);
	int ProcessOption();
	int ProcessDescribe();
	int ProcessSetup();
	int ProcessPlay();
	int ProcessPause();
	int ProcessSetParam();
	int ProcessGetParam();

private:
	void ReadFromSource(MediaPacket packet);

private:
	std::string sdp;
	RtspRequest rtspRequest;

//	RtspSource *_source;
//	RTSPConsumer *_consumer;
	//MediaFile *_file;
	TcpConnectionPtr tcpconn;
private:
	std::shared_ptr<FlvReader> flvReader;
};





}
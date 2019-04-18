#include "HttpFlvSession.h"

namespace base {
	


int HttpFlvSession::OnRecv(std::shared_ptr<TcpConnection> conn)
{
	//printf("")
	char a[4096] = { 0 };
	memcpy(a, conn->inputBuffer_.GetReadPtr(), conn->inputBuffer_.Size());
	printf("http is %s\n", a);
	request_.Parse(tcpconn_->inputBuffer_);



	return 0;
}

void HttpFlvSession::ProcessHttp()
{
	std::string url = request_.GetUrl();
	int pos = url.find_last_of('/');
	app = url.substr(0, pos);
	stream = url.substr(pos + 1);

//	auto src = RtmpSource::FindMediaSource(stream);
	auto src = RtmpSource::FindMediaSource("ss");

	HttpResponse http_response;
	std::string response = http_response.BuildHeader();
	tcpconn_->Send(response);



	flvEncoder_ = std::make_shared<FlvEncoder>();
	flvEncoder_->SetSendCallback(std::bind(&HttpFlvSession::SendTag, this, std::placeholders::_1, std::placeholders::_2));
	flvEncoder_->WriteHeader();

	std::weak_ptr<HttpFlvSession> wksession = shared_from_this();
	//	src->SetReadCallback(std::bind(&RtmpSession::ReadFromSource, 
	//	std::weak_ptr<RtmpSession>(shared_from_this()), std::placeholders::_1));
	src->StartRead([wksession](MediaPacket packet) {
		std::shared_ptr<HttpFlvSession> session = wksession.lock();
		if (session)
		{
			session->ReadFromSource(packet);
		}
	});
}

void HttpFlvSession::SendTag(void* data, size_t size)
{
	tcpconn_->Send(data, size);
}


void HttpFlvSession::ReadFromSource(MediaPacket packet)
{
	//RtmpTagHeader header;
	flvEncoder_->WriteFlvTag(packet);
}


	


}
#include "base/TcpConnection.h"

#include <functional>

namespace base {
	
TcpConnection::TcpConnection(EventMessageLoop* loop, int sockfd) 
	: loop_(loop), socket_(sockfd),
	  inputBuffer_(5* 1024 * 1024), outputBuffer_(5 * 1024 * 1024), waitingWrite_(false), state_(kDmmuy), 
	  highWaterWriteWarning_(~0), writeable_(true)
{
	socket_.SetNonBlocking();
	socket_.SetNoDelay();
	socket_.SetSocketSendBufSize(1024 * 1024 * 1);
//	loop_->AddEvent(sockfd, EventRead|EventError, std::bind(&TcpConnection::HandleEvent, this, std::placeholders::_1));
}

void TcpConnection::HandleClose()
{
	if (state_ != kOpen)
	{
		return;
	}
	loop_->RemoveEvent(socket_.Fd());
	if (disConnectCallback_)
	{
		disConnectCallback_(shared_from_this());
	}
	if (closeCallback_)
	{
		closeCallback_(shared_from_this());
	}
	state_ = kCLosed;
}

void TcpConnection::Start()
{
	state_ = kOpen;
	socket_.SetNonBlocking();
	loop_->AddEvent(socket_.Fd(), EventRead | EventError, std::bind(&TcpConnection::HandleEvent, this, std::placeholders::_1));
}

void TcpConnection::SendC(int len)
{
//	loop_->PostDelayTask(std::bind(&TcpConnection::SendTemp, shared_from_this(), len), 0);
	//loop_->PostDelayTask(std::bind(&TcpConnection::SendTemp, shared_from_this(), len), 0);
	if (len > sendRate_)
	{
		int64_t wait = 0;

		int sendLen = len;
		while (sendLen)
		{
			int real = sendLen < sendRate_ ? sendLen : sendRate_;
			loop_->PostDelayTask(std::bind(&TcpConnection::SendTemp, shared_from_this(), real), wait);
			wait += 1000000;
			sendLen -= real;
		}
	}
	else
	{
		loop_->PostDelayTask(std::bind(&TcpConnection::SendTemp, shared_from_this(), len), 0);
	}
}

void TcpConnection::SendB(void *data, int len)
{
	if (!loop_->BelongsToCurrentThread())
	{
		//	unsigned sendLen = 0;
		//	socket_.Send(data, len, &sendLen);
		loop_->PostTask(std::bind(&TcpConnection::SendB, shared_from_this(), data, len));
		return;
	}

	sendLimterBuffer_.Write(static_cast<char *>(data), len);
	int64_t waitms = sendLimter_.Accquire(len);
	if (waitms == 0)
	{
		//Send(data, len);
		SendC(len);
	}
	else
	{
	//	loop_->PostDelayTask(std::bind(&TcpConnection::SendTemp, shared_from_this(), len), waitms);
		loop_->PostDelayTask(std::bind(&TcpConnection::SendC, shared_from_this(), len), waitms);
	}
}



void TcpConnection::SendTemp(int len)
{
	char *buffer = (char *)malloc(len);

	sendLimterBuffer_.Read(buffer, len);
	Send(buffer, len);
	delete buffer;
	//Send(sendLimterBuffer_.GetDataPtr(), len);
	//sendLimterBuffer_.AddReadPos(len);
}


void TcpConnection::Send(IOBufferPtr buffer)
{
	//if (!loop_->BelongsToCurrentThread())
//	{
		Send(buffer->Data(), buffer->Size());
//	}
}

void TcpConnection::Send(std::string& data)
{
	Send((void *)(data.c_str()), data.size());
}

void TcpConnection::Send(void *data, int len)
{
	assert(data);
	if (!loop_->BelongsToCurrentThread())
	{
		//	unsigned sendLen = 0;
		//	socket_.Send(data, len, &sendLen);
		void (TcpConnection::*fp)(IOBufferPtr buffer) = &TcpConnection::Send;
		IOBufferPtr buffer(new IOBuffer(static_cast<char *>(data), static_cast<size_t>(len)));
		loop_->PostTask(std::bind(fp, shared_from_this(), buffer));
		return;
	}

	if (state_ != kOpen)
	{
		return ;
	}

	unsigned int sendLen = 0;
	if (!waitingWrite_  && outputBuffer_.Empty())
	{
		int ret = socket_.Send(data, len, &sendLen);

		if (ret == kSocketConnectRest) //windows 
		{
			HandleClose();
			return;
		}
		else if (ret == kSocketOk)
		{
			if (sendLen == len)
			{
				return;
			}
		}
	}

	outputBuffer_.Write(static_cast<char*>(data) + sendLen, len - sendLen);
	if (outputBuffer_.Size() >= highWaterWriteWarning_)
	{
		writeable_ = false;
	}
	if (!waitingWrite_)
	{
		waitingWrite_ = true;
		loop_->ModifyEvent(socket_.Fd(), EventRead | EventWrite);
	}
}

void TcpConnection::HandleWrite()
{
	if (!waitingWrite_ || outputBuffer_.Empty())
	{
		//
		printf("error---\n");
	}

//	printf("33333333333333333--\n");
	struct iovec buffers[2];
	outputBuffer_.GetReadPos(&buffers[0], &buffers[1]);

	/*int senda = 0;
	if (buffers[1].iov_len == 0)
	{
		senda = 1;
	}
	else
	{
		senda = 2;
	}*/

	unsigned int recvLen = 0;
	int ret = socket_.WriteV(buffers, 2, &recvLen);
	if(ret == kSocketConnectRest)
	{
		HandleClose();
	}
	else if (ret == kSocketOk)
	{
	/*	if (buffers[1].iov_len == 0)
		{
			printf("dierduan wei 0");
		}
		else
		{
			printf("dierduan wei 1");
		}*/
		if (recvLen == 0)
		{
			int c = 0;
		}
		printf("yao send is %d\n", buffers[0].iov_len + buffers[1].iov_len);
		outputBuffer_.AddReadPos(recvLen);
		if (outputBuffer_.Size() < highWaterWriteWarning_)
		{
			writeable_ = true;
		}

		printf("send len is %d\n", recvLen);

		if (outputBuffer_.Empty())
		{
			waitingWrite_ = false;
			loop_->ModifyEvent(socket_.Fd(), EventRead);
		}
	}
	else if(ret == kSocketPending)
	{
	//	printf("send len is %d\n", buffers[0].iov_len);
//		printf("recvLen is %d\n", recvLen);
	//	printf("write error or pending--\n");
	}
	else
	{
		int a = 1;
	}

}
	
void TcpConnection::HandleEvent(int type)
{
	if(type & EventRead)
	{
		HandleRead();
	}
	if (type & EventWrite)
	{
		HandleWrite();
	}
	
}

void TcpConnection::HandleRead()
{
	struct iovec buffers[2];
	unsigned int recvLen = 0;
	
	while (1)
	{
		//bool reselt = inputBuffer_.GetWritePos(&buffers[0], &buffers[1]);
		while (!inputBuffer_.GetWritePos(&buffers[0], &buffers[1]))
		{
			inputBuffer_.Resize(2 * inputBuffer_.Capacity());
		}
		unsigned int len = 0;
		int ret = socket_.ReadV(buffers, 2, &len);
		if (ret == kSocketConnectRest)
		{
			HandleClose();
			return;
		}
		else if (ret != 0)
		{
			break;
		}
		recvLen += len;
		inputBuffer_.AddWritePos(len);

		if (!inputBuffer_.Full())
		{
			break;
		}
	}

	if (recvLen && readCallback_)
	{
		readCallback_(shared_from_this());
	}
}

void TcpConnection::SetAttr(std::string key, BoostAny any)
{
	attrmap_.insert(make_pair(key, any));
}

BoostAny TcpConnection::GetAttr(std::string key)
{
	auto any = attrmap_.find(key);
	if (any != attrmap_.end())
	{
		return any->second;
	}
	else
	{
		return BoostAny();
	}
}
	
	
	
	
	
	
}
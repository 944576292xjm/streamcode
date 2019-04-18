#ifndef _BASE_TCP_CONNECTION_H
#define _BASE_TCP_CONNECTION_H

#include "base/EventMessageLoop.h"
#include "base/Callback.h"
#include "base/RingBuffer.h"
#include "base/BoostAny.h"
#include <unordered_map>
#include <atomic>
#include "base/RateLimiter.h"
#include "base/IOBuffer.h"

namespace base {

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(EventMessageLoop* loop, int sockfd);

	void SetReadCallback(NewConnectionCallback callback) { readCallback_ = std::move(callback); }
	void SetCloseCallback(NewConnectionCallback callback) { closeCallback_ = std::move(callback);}
	void SetDisConnectCallback(NewConnectionCallback callback) { disConnectCallback_ = callback; }

	void Start();
	//void SetReadCallback(EventCallback readCallback) { readCallback_ = readCallback; }
	//void SetCloseCallback(EventCallback closeCallback) { closeCallback_ = closeCallback; }

	void SetAttr(std::string key, BoostAny any);

	BoostAny GetAttr(std::string key);

	void Send(std::string& data);
	void Send(void *data, int len);
	void Send(IOBufferPtr buffer);

	void SetWriteWarningSize(int size) { highWaterWriteWarning_ = size; }

	bool Writeable() {return writeable_;}

	void SetSendRate(int limit)
	{
		sendRate_ = limit;
		sendLimter_.SetNewRate(limit);
		//std::shared_ptr<RingBuffer> sendLimterBuffer_;
	}

	void SendC(int len);
	void SendB(void *data, int len);
	void SendTemp(int len);

	void closeSendRate()
	{
		
	}
	
	//bool Writeable();

	//bool WriteWaiting();

	RingBuffer inputBuffer_;
	RingBuffer outputBuffer_;
private:
	void HandleEvent(int type);
	void HandleWrite();
	void HandleRead();
	void HandleClose();

private:
	size_t highWaterWriteWarning_;
	bool writeable_;

private:
	//ReadCallback readCallback_;
	typedef std::unordered_map<std::string, BoostAny> AttrMap;

	NewConnectionCallback readCallback_;
	NewConnectionCallback closeCallback_;
	NewConnectionCallback disConnectCallback_;

	TcpSocket socket_;
	EventMessageLoop* loop_;

	//EventCallback readCallback_;
	//EventCallback writeCallback_;
	//EventCallback closeCallback_;

//	RingBuffer inputBuffer_;
	AttrMap attrmap_;

	RateLimiter sendLimter_;
	int sendRate_;
	//RingBuffer sendLimterBuffer_ = 1024 * 1024 * 1024;
	RingBuffer sendLimterBuffer_ = 5;
private:
	typedef enum
	{
		kDmmuy,
		kOpen,
		kCLosed,
	}State;
	bool waitingWrite_;
	std::atomic<int> state_;
};

}

#endif
#ifndef __BASE_SOCKET_PIPE_H
#define __BASE_SOCKET_PIPE_H

#include <memory>
#include "base/Socket.h"
#include "base/SocketFactory.h"
#include "base/EndPoint.h"

namespace base {

class SocketPipe
{
public:
	SocketPipe();
	int Write(const void *buf, unsigned int size);
	int Read(void *buf, unsigned int size);
	int writeFd() { return writeSocket_.Fd(); }
	int readFd() { return readSocket_.Fd(); }
private:
	ClientSocket writeSocket_;
	ClientSocket readSocket_;
};






}

#endif
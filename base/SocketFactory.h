#ifndef _BASE_SOCKET_FACTORY_H
#define _BASE_SOCKET_FACTORY_H

#include "base/Socket.h"

namespace base {

class SocketFactory
{
public:
	static ListenSocket* CreateListenSocket(EndPoint endPoint)
	{
		ListenSocket* socket = new ListenSocket(endPoint);
		return socket;
	}

	static TcpSocket* CreateTcpSocket(int fd)
	{
		TcpSocket* socket = new TcpSocket(fd);
		return socket;
	}

	static ClientSocket* CreateClientSocket(EndPoint endPoint = EndPoint())
	{
		ClientSocket* socket = new ClientSocket(endPoint);
		return socket;
	}
	
};

}


#endif
#ifndef _BASE_ENDPOINT_H
#define _BASE_ENDPOINT_H

#include <string>

#ifdef __Win32__
#include <winsock2.h>  
#elif
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <netdb.h>   
#include <arpa/inet.h>  
#endif

namespace base
{
	
class EndPoint
{
public:
	EndPoint() : ip_(), port_(-1), ipv4_(true) {}
	EndPoint(unsigned short port,const char *addr = "0.0.0.0", bool ipv4 = true)
		: ip_(addr), port_(port), ipv4_(ipv4) {}

	std::string IpAddress() const
	{
		return ip_;
	}
	
	int Port() const
	{
		return port_;
	}

	bool IsIpv4() const
	{
		return ipv4_;
	}

	bool ToSockAddr(struct sockaddr *address);
	bool FromSockAddr(struct sockaddr *address);

private:
	std::string ip_;
	int port_;
	bool ipv4_;
};


}

#endif
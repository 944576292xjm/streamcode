#include "base/EndPoint.h"

namespace base
{
	

bool EndPoint::ToSockAddr(struct sockaddr *address) 
{
	if(port_ < 0 || ip_.empty())
	{
		return false;
	}
	
	if(ipv4_)
	{
		struct sockaddr_in *in_address = reinterpret_cast<struct sockaddr_in*>(address);
		in_address->sin_family = AF_INET;
		in_address->sin_port = htons(port_);
		in_address->sin_addr.s_addr = inet_addr(ip_.c_str());
	}
	return true;
}

bool EndPoint::FromSockAddr(struct sockaddr* address)
{
	auto in_address = reinterpret_cast<const struct sockaddr_in*>(address);
	port_ = ntohs(in_address->sin_port);
	ip_ = inet_ntoa(in_address->sin_addr);
	ipv4_ = true;
	return true;
}

	

	
}
#pragma once


#if 0
namespace base {
	
class Packet{
public:

	Packet(std::shared_ptr<Buffer> buffer = NULL) : _pBuffer(buffer), frame(FRAME_UNSPECIFIED), timeoffset(0){}
	/*Packet(Buffer &&buffer) : type(FRAME_UNSPECIFIED), timeoffset(0), _pBuffer(buffer)
	{

	}*/
	bool isEmpty()
	{
		if (!_pBuffer)
		{
			return true;
		}
		return false;
	}

	Frame  frame;
	unsigned int timeoffset;
	std::shared_ptr<Buffer> _pBuffer;

	Buffer* operator->() { return _pBuffer.get();}

	void reset(Buffer *buffer = NULL)
	{
		_pBuffer.reset(buffer);
	}
	//operator const std::shared_ptr<Buffer>&() { return _pBuffer; }
	
//	Buffer*	operator->() { return _pBuffer.get(); }
//	operator std::shared_ptr<Buffer>() { return _pBuffer; }

};

}

#endif
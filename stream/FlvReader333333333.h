#pragma once
#include "base.h"
#include <memory>
#include "MediaReader.h"
#include "AMF.h"

namespace base {


class FlvReader : public MediaReader
{
public:
	FlvReader();
	~FlvReader();

	virtual int Decode(std::shared_ptr<Buffer> buffer);
private:
	bool firstRead;
	AMFType	_type;
	unsigned int      _size;
};


}
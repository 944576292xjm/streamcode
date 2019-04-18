#pragma once
#include <memory>
#include <string>
#include "BinaryReader.h"
#include "AMF.h"
#include <map>

namespace base {





	class AMFReader : public BinaryReader
	{
	public:
		AMFReader(unsigned char* data, unsigned int size, bool bigEndian = true)
			: BinaryReader(data, size, bigEndian) {}
		~AMFReader() {}

		std::string AMF_ReadString();

		int AMF_ReadNull();
		double    AMF_ReadNumber();
		AMFValue AMFReadValue();

		AMFValue AMF_ReadObject();

	private:

		
	//	typedef std::vector<AMFValue> arrayType;

	};

	

}
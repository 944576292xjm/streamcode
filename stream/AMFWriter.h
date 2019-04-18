#pragma once
#include <memory>
#include <string>
#include "Buffer.h"
#include "AMF.h"
#include <map>

namespace base {





	class AMFWriter 
	{
	public:
		AMFWriter();
		~AMFWriter();

		int AMF_WriteString(std::string str);
		int AMF_WriteNumber(double number);
		int AMF_Write16(unsigned short n);
		int AMF_Write32(unsigned int n);
		//int AMF_WriteNumber(double number);
		int AMF_WriteBoolean(bool b);
		int AMFWriteValue(AMFValue value);
		int AMF_WriteNull();
		int AMF_WriteUndefined();

		int AMF_WriteObject(AMFValue value);

		const char* Data(){ return (const char *)buffer.data(); }
		unsigned int Size(){ return buffer.size(); }
	private:

		Buffer buffer;
		//	typedef std::vector<AMFValue> arrayType;

	};



}
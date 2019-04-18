#include "AMFReader.h"
#include <memory>

namespace base {


/*
AMFReader::AMFReader(const unsigned char* data, unsigned int size, bool bigEndian) : BinaryReader(data,size, bigEndian)
{
}*/


AMFValue AMFReader::AMFReadValue()
{
	//unsigned char_t type = front();
	//unsigned char type = Read8();
	unsigned char type = *Current();
	/*if (version == 3) {
		switch (type) {
		case AMF3_STRING:
			//return load<std::string>();
			return AMF_ReadString();
		case AMF3_NUMBER:
		//	return load<double>();
			return 1;
		case AMF3_INTEGER:
			return load<int>();
		case AMF3_FALSE:
			pos++;
			return false;
		case AMF3_TRUE:
			pos++;
			return true;
		case AMF3_OBJECT:
			return load_object();
		case AMF3_ARRAY:
			return load_ecma();
		case AMF3_NULL:
			pos++;
			return AMF_NULL;
		case AMF3_UNDEFINED:
			pos++;
			return AMF_UNDEFINED;
		default:

		}
	}*/
	//else {
	switch (type) {
	case AMF0_STRING:
		return AMF_ReadString();
	case AMF0_NUMBER:
		return AMF_ReadNumber();
	case AMF0_BOOLEAN:
		//	return AMF_ReadBoollean();
	case AMF0_OBJECT:
		//return load_object();
	case AMF0_ECMA_ARRAY:
		//return load_ecma();
	case AMF0_NULL:
		//pos++;
		//return AMF_NULL;
	case AMF0_UNDEFINED:
		//pos++;
		//return AMF_UNDEFINED;
	case AMF0_STRICT_ARRAY:
		//return load_arr();
	default:
		break;
		//throw std::runtime_error(
			//StrPrinter << "Unsupported AMF type:" << (int)type << endl);
	}
	//	}
	return 0;
}

double  AMFReader::AMF_ReadNumber()
{
	unsigned char type(Read8());

	if (Available() < 8)
	{
		throw std::runtime_error("Not enough data");
		return 0;
	}

	uint64_t val = Read64();



	double n = 0;
	memcpy(&n, &val, 8);



	/*uint64_t val = Read32()<< 32 | Read32();
	//Next(8);
	double n = 0.0;
	memcpy(&n, &val, 8);*/
	//pos += 8;
	return n;
	//return 0;
}

int AMFReader::AMF_ReadNull()
{
	unsigned char type = Read8();
	if (type != AMF0_NULL)
	{
		return -1;
	}
	return 0;
}

std::string AMFReader::AMF_ReadString()
{
	std::string tmp;

	unsigned char type = Read8();
	if (type != AMF0_STRING)
	{
		return "";
	}

	unsigned int len = Read16();

	//Read(tmp,len);
	//tmp.assign("1");
	if (len <= Available() && len)
	{
		tmp.assign((const char *)Current(), len);
		Next(len);
		return tmp;
	}
	return "";
}

AMFValue AMFReader::AMF_ReadObject()
{
	AMFValue object(AMFValue::AMFVALUE_OBJECT);
	unsigned char type = Read8();
	if (type != AMF0_OBJECT)
	{
		//return "";
	}
	while (1)
	{
		unsigned int keyLen = Read16();
		//	if (keyLen > Available())
		//	{

		//	}
		std::string key((const char *)Current(), keyLen);
		Next(keyLen);

		if (key.empty())
		{
			break;
		}
		//	std::string key = AMF_ReadString();
			//std::string key((const char *)Current(), keyLen);
			//Next(keyLen);
		//	std::string key = load_key();
		//	if (key.empty())
		//		break;
		AMFValue value = AMFReadValue();
		object.add(key, value);
	}
	/*	if (pop_front() != AMF0_OBJECT_END) {
			throw std::runtime_error("expected object end");
		}*/
	if (Read8() != AMF0_OBJECT_END)
	{
		printf("1111\n");
	}
	return object;

}


}



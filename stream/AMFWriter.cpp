#include "AMFWriter.h"
#include <memory>
#include "BinaryReader.h"
namespace base {



AMFWriter::AMFWriter() 
{
}

AMFWriter::~AMFWriter()
{
}



int AMFWriter::AMFWriteValue(AMFValue value) 
{

	switch (value.type())
	{
	case AMFValue::AMFVALUE_STRING:
		AMF_WriteString(value.GetString());
		break;
	case AMFValue::AMFVALUE_NUMBER:
		AMF_WriteNumber(value.GetNumber());
		break;
	case AMFValue::AMFVALUE_INTEGER:
		//AMF_WriteNumber(value.Getint);
		break;
	case AMFValue::AMFVALUE_BOOLEAN:
		AMF_WriteBoolean(value.GetBoolean());
		break;
	case AMFValue::AMFVALUE_OBJECT:
		AMF_WriteObject(value);
		break;
	case AMFValue::AMFVALUE_NULL:
		AMF_WriteNull();
		break;
	case AMFValue::AMFVALUE_UNDEFINED:
		AMF_WriteUndefined();
		break;

		/*case AMF_NULL:
			*this << nullptr;
			break;
			case AMF_UNDEFINED:
			this->write_undefined();
			break;
			case AMF_STRICT_ARRAY: {
			buf += char(AMF0_STRICT_ARRAY);
			unsigned int_t sz = htonl(value.getArr().size());
			buf.append((char *)&sz, 4);
			for (auto &val : value.getArr()) {
			*this << val;
			}
			//write_key("");
			//buf += char(AMF0_OBJECT_END);
			}
			break;
			}
			*/
	}

	return 0;
}

int AMFWriter::AMF_WriteObject(AMFValue value)
{
	unsigned char type = AMF0_OBJECT;
	buffer.Append(&type, 1);
	for (auto &pr : value.GetObject()) 
	{
		//write_key(pr.first);
		unsigned short str_len = BinaryReader::Flip16(pr.first.size());
		buffer.Append((char *)&str_len, 2);
		buffer.Append(pr.first.c_str(), pr.first.size());
		AMFWriteValue(pr.second);
	}
	//unsigned char end = AMF0_OBJECT_END;
	
	//unsigned int end = 0x090000;
	 unsigned int end = BinaryReader::Flip24(AMF0_OBJECT_END);
	buffer.Append((char *)&end, 3);
//	buffer.Append((char *)"00", 2);
	//buffer.Append((char *)&end, 1);
	/*std::string end("");
	unsigned short str_len = BinaryReader::Flip16(end);
	buf.append((char *)&str_len, 2);
	buf += s;
	write_key("");*/
	return 0;
}

int  AMFWriter::AMF_WriteNumber(double number)
{
	unsigned char type = AMF0_NUMBER;
	buffer.Append(&type, 1);
	uint64_t encoded = 0;
	memcpy(&encoded, &number, 8);
//	unsigned int val = BinaryReader::Flip32(encoded >> 32);
	//buffer.Append((char *)&val, 4);
	//val = BinaryReader::Flip32((encoded << 32) >> 32);
	uint64_t val = BinaryReader::Flip64(encoded);
	buffer.Append((char *)&val, 8);
	
	return 0;
}

int AMFWriter::AMF_Write16(unsigned short n)
{
	unsigned short val = BinaryReader::Flip16(n);
	buffer.Append((char *)&val, 2);
	return 0;
}
int AMFWriter::AMF_Write32(unsigned int n)
{
	unsigned int val = BinaryReader::Flip32(n);
	buffer.Append((char *)&val, 4);
	return 0;
}

int AMFWriter::AMF_WriteBoolean(bool b)
{
	unsigned char type = AMF0_BOOLEAN;
	buffer.Append(&type, 1);
	buffer.Append(&b, 1);
	return 0;
}

int AMFWriter::AMF_WriteNull()
{
	unsigned char type = AMF0_NULL;
	buffer.Append(&type, 1);
	return 0;
}

int AMFWriter::AMF_WriteUndefined()
{
	unsigned char type = AMF0_UNDEFINED;
	buffer.Append(&type, 1);
	return 0;
}


int AMFWriter::AMF_WriteString(std::string str)
{

	if (!str.empty()) {
		//buf += char(AMF0_STRING);
		unsigned char type = AMF0_STRING;
		buffer.Append(&type, 1);
		unsigned short str_len =BinaryReader::Flip16(str.size());
		buffer.Append(&str_len, 2);
		buffer.Append(str.c_str(), str.size());
	}
	else {
		return -1;
		//unsigned char type = AMF0_NULL;
		//buffer.Append(&type, 1);
	}
	return 0;
}


}
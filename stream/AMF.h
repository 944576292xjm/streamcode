#pragma once

#include <map>
#include <string>

namespace base {


enum AMFType {
	AMF_TYPE_EMPTY = 0x00,
	AMF_TYPE_CHUNKSIZE = 0x01,
	AMF_TYPE_ABORT = 0x02,
	AMF_TYPE_ACK = 0x03,
	AMF_TYPE_RAW = 0x04,
	AMF_TYPE_WIN_ACKSIZE = 0x05,
	AMF_TYPE_BANDWIDTH = 0x06,
	AMF_TYPE_AUDIO = 0x08,
	AMF_TYPE_VIDEO = 0x09,
	AMF_TYPE_DATA_AMF3 = 0x0F,
	AMF_TYPE_INVOCATION_AMF3 = 0x11,
	AMF_TYPE_DATA = 0x12,
	AMF_TYPE_INVOCATION = 0x14
};

// AMF0��������;
typedef enum
{
	AMF0_NUMBER = 0,         // ����(double);
	AMF0_BOOLEAN,            // ����;
	AMF0_STRING,             // �ַ���;
	AMF0_OBJECT,             // ����;
	AMF0_MOVIECLIP,          // ����,δʹ��;
	AMF0_NULL,               // null;
	AMF0_UNDEFINED,          // δ����;
	AMF0_REFERENCE,          // ����;
	AMF0_ECMA_ARRAY,         // ����;
	AMF0_OBJECT_END,         // �������;
	AMF0_STRICT_ARRAY,       // �ϸ������;
	AMF0_DATE,               // ����;
	AMF0_LONG_STRING,        // ���ַ���;
	AMF0_UNSUPPORTED,        // δ֧��;
	AMF0_RECORDSET,          // ����,δʹ��;
	AMF0_XML_DOC,            // xml�ĵ�;
	AMF0_TYPED_OBJECT,       // �����͵Ķ���;
	AMF0_AVMPLUS,            // ��Ҫ��չ��AMF3;
	AMF0_INVALID = 0xff      // ��Ч��;
}AMF0DataType;




class AMFValue {
public:

	enum AMFVALUEType {
		AMFVALUE_NUMBER,
		AMFVALUE_INTEGER,
		AMFVALUE_BOOLEAN,
		AMFVALUE_STRING,
		AMFVALUE_OBJECT,
		AMFVALUE_NULL,
		AMFVALUE_UNDEFINED,
		AMFVALUE_ECMA_ARRAY,
		AMFVALUE_STRICT_ARRAY,
	};

	typedef std::map<std::string, AMFValue> ValueMap;

	AMFValue(AMFVALUEType type = AMFVALUE_NULL);
	AMFValue(const char *s);
	AMFValue(const std::string &s);
	AMFValue(double n);
	AMFValue(int i);
	AMFValue(bool b);
	//AMFValue(const AMFValue &from);
//	AMFValue(AMFValue &&from);
	//AMFValue &operator =(const AMFValue &from);
	//AMFValue &operator =(AMFValue &&from);
	~AMFValue();

	AMFVALUEType type(){ return m_type; }
	void add(const std::string &s, const AMFValue &val);

	const AMFValue operator[](const char *str) 
	{
		if (m_type != AMFVALUE_OBJECT && m_type != AMFVALUE_ECMA_ARRAY) 
		{
			//throw std::runtime_error("AMF not a object");
		}
		auto value = m_value.object->find(str);
		if (value == m_value.object->end()) {
			//static AMFValue val(AMF_NULL);
			//return val;
			return AMFValue(AMFVALUE_NULL);
		}
		return value->second;
	}

	

	const std::string GetString();
	const double      GetNumber();
	const int		Getint();
	const bool        GetBoolean();
	const ValueMap  GetObject();

private:


	inline void init();

	AMFVALUEType m_type;

	union {
		std::string *string;
		double number;
		int integer;
		bool boolean;
		ValueMap *object;
		//arrayType *array;
	} m_value;

};


}
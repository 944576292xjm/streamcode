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

// AMF0数据类型;
typedef enum
{
	AMF0_NUMBER = 0,         // 数字(double);
	AMF0_BOOLEAN,            // 布尔;
	AMF0_STRING,             // 字符串;
	AMF0_OBJECT,             // 对象;
	AMF0_MOVIECLIP,          // 保留,未使用;
	AMF0_NULL,               // null;
	AMF0_UNDEFINED,          // 未定义;
	AMF0_REFERENCE,          // 引用;
	AMF0_ECMA_ARRAY,         // 数组;
	AMF0_OBJECT_END,         // 对象结束;
	AMF0_STRICT_ARRAY,       // 严格的数组;
	AMF0_DATE,               // 日期;
	AMF0_LONG_STRING,        // 长字符串;
	AMF0_UNSUPPORTED,        // 未支持;
	AMF0_RECORDSET,          // 保留,未使用;
	AMF0_XML_DOC,            // xml文档;
	AMF0_TYPED_OBJECT,       // 有类型的对象;
	AMF0_AVMPLUS,            // 需要扩展到AMF3;
	AMF0_INVALID = 0xff      // 无效的;
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
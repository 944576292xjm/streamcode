
#include "AMF.h"


namespace base {

	inline void AMFValue::init() {
		switch (m_type) {
		case AMFVALUE_OBJECT:
		case AMFVALUE_ECMA_ARRAY:
			m_value.object = new ValueMap;
			break;
		case AMFVALUE_STRING:
			m_value.string = new std::string;
			break;
		case AMFVALUE_STRICT_ARRAY:
		//	m_value.array = new arrayType;
			break;

		default:
			break;
		}

	}
	AMFValue::AMFValue(AMFVALUEType type) :m_type(type)
	{
		init();
	}


AMFValue::~AMFValue() {
	//destroy();
}

AMFValue::AMFValue(const char *s) :m_type(AMFVALUE_STRING) 
{
	init();
	*m_value.string = s;
}




	AMFValue::AMFValue(const std::string &s) :m_type(AMFVALUE_STRING)
	{
		init();
		*m_value.string = s;
	}

	AMFValue::AMFValue(double n) : m_type(AMFVALUE_NUMBER) 
	{
		init();
		m_value.number = n;
	}

	AMFValue::AMFValue(int i) : m_type(AMFVALUE_INTEGER) 
	{
		init();
		m_value.integer = i;
	}

	AMFValue::AMFValue(bool b) : m_type(AMFVALUE_BOOLEAN) 
	{
		init();
		m_value.boolean = b;
	}

	void AMFValue::add(const std::string &s, const AMFValue &val) 
	{
		if (m_type != AMFVALUE_OBJECT && m_type != AMFVALUE_ECMA_ARRAY) 
		{
			//throw std::runtime_error("AMF not a object");
		}
		m_value.object->emplace(s, val);
	}

	const std::string AMFValue::GetString()
	{
		if (m_type != AMFVALUE_STRING){
			//throw std::runtime_error("AMF not a string");
			return "";
		}
		return *m_value.string;
	}

	const double  AMFValue::GetNumber()
	{
		if (m_type != AMFVALUE_NUMBER && m_type != AMFVALUE_INTEGER){
			//throw std::runtime_error("AMF not a string");
			//return "";
			//throw std::runtime_error("AMF not a number");
		}
		return m_value.number;
	}
	const int  AMFValue::Getint()
	{
		return m_value.integer;
	}
	const bool    AMFValue::GetBoolean()
	{
		return m_value.boolean;
	}

	const AMFValue::ValueMap  AMFValue::GetObject()
	{
		return *m_value.object;
	}



}
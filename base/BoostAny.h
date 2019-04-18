#ifndef _BASE_ANY_H
#define _BASE_ANY_H

#include <algorithm>
#include <typeinfo>

namespace base {

class BoostAny
{
	public: 
		BoostAny()
		  : content(0)
		{
		}

		template<typename ValueType>
		BoostAny(const ValueType & value)
		  : content(new holder<ValueType>(value))
		{
		}

		BoostAny(const BoostAny & other)
		  : content(other.content ? other.content->clone() : 0)
		{
		}

		~BoostAny()
		{
			delete content;
		}

	public:
		BoostAny & swap(BoostAny & rhs)
		{
			std::swap(content, rhs.content);
			return *this;
		}

		template<typename ValueType>
		BoostAny & operator=(const BoostAny & rhs)
		{
			BoostAny(rhs).swap(*this);
			return *this;
		}

		BoostAny & operator=(const BoostAny & rhs)
		{
			BoostAny(rhs).swap(*this);
			return *this;
		}

	public: 
		bool empty() const
		{
			return !content;
		}

		const std::type_info & type() const
		{
			return content ? content->type() : typeid(void);
		}

	public: 
		class placeholder
		{
		public: 
			virtual ~placeholder()
			{
			}

		public: 
			virtual const std::type_info & type() const = 0;
			virtual placeholder * clone() const = 0;
		};

		template<typename ValueType>
		class holder : public placeholder
		{
		public: 
			holder(const ValueType & value)
			  : held(value)
			{
			}

		public: 
			virtual const std::type_info & type() const
			{
				return typeid(ValueType);
			}

			virtual placeholder * clone() const
			{
				return new holder(held);
			}

		public: 
			ValueType held;
		};

	private: 
		template<typename ValueType>
		friend ValueType * any_cast(BoostAny *);
			
	public:
		placeholder* content;
	};

	class bad_any_cast : public std::bad_cast
	{
	public:
		virtual const char * what() const throw()
		{
			return "bad_any_cast: "
				   "failed conversion using any_cast";
		}
	};

	template<typename ValueType>
	ValueType * any_cast(BoostAny * operand)
	{
		return operand && operand->type() == typeid(ValueType)
					? &static_cast<BoostAny::holder<ValueType> *>(operand->content)->held
					: 0;
	}

	template<typename ValueType>
	const ValueType* any_cast(const BoostAny* operand)
	{
		return any_cast<ValueType>(const_cast<BoostAny*>(operand));
	}

	template<typename ValueType>
	ValueType any_cast(const BoostAny & operand)
	{
		const ValueType* result = any_cast<ValueType>(&operand);
		if(!result)
			throw bad_any_cast();
		return *result;
	}

};

#endif 

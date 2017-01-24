#pragma once

LN_NAMESPACE_BEGIN

enum class NlVariantType
{
	Null = 0,
	Bool = 1,
	Int32 = 2,
	Float = 3,
	String = 4,
	Vector3 = 5,
};

/**
	@brief	
*/
class NlVariant
	: public Object
{
public:
	NlVariant();
	~NlVariant();
	
	NlVariantType GetType() const { return m_type; }


	void SetValue(int32_t value) { m_type = NlVariantType::Int32; m_int32 = value; }


	template<typename T>
	T GetValue() { return T(); }

	template<>
	int32_t GetValue<int32_t>() { return m_int32; }

	//NlVariant(const NlVariant& other);

private:
	NlVariantType	m_type;
	union/* U*/
	{
		bool		m_bool;
		int32_t			m_int32;
		float		m_float;
		//std::string	m_string;
		String		m_string;
		Vector3		m_vector3;
		
		//U() { }
		//~U() { }

		//U(const U& other)
		//{

		//}
	};
};

LN_NAMESPACE_END


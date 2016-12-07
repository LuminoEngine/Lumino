#pragma once

LN_NAMESPACE_BEGIN

enum class NlVariantType
{
	Bool,
	Int,
	Float,
	String,
	Vector3,
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


	//NlVariant(const NlVariant& other);

private:
	NlVariantType	m_type;
	union/* U*/
	{
		bool		m_bool;
		int			m_int;
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


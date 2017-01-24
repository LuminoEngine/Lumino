
#pragma once
#include "Token.h"

namespace ln
{
namespace fl
{

/**
	@brief
*/
class TranslationUnit
{
public:
	TranslationUnit();
	virtual ~TranslationUnit();

private:
	TokenManager	m_tokenManager;
	TokenList		m_tokenList;
};

} // namespace fl
} // namespace ln

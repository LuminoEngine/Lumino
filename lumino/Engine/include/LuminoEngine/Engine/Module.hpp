#pragma once
#include "Common2.hpp"

namespace ln {

class Module : public RefObject
{
public:

protected:
	Module();
	virtual ~Module();

private:
	EngineContext2* m_context;

	friend class EngineContext2;
};

} // namespace ln

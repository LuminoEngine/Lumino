#pragma once

namespace ln {

class RenderFeature
	: public Object
{
public:
	virtual void flush() = 0;

private:
};

} // namespace ln


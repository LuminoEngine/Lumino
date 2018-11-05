#pragma once

namespace ln {
class GraphicsContext;

class RenderFeature
	: public Object
{
public:
	virtual void flush(GraphicsContext* context) = 0;

private:
};

} // namespace ln


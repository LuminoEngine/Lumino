
#pragma once

namespace ln {
class Material;
class RenderingContext;
namespace detail {

class InternalSkyBox
	: public Object
{
public:
	InternalSkyBox();
	void init();
	void render(RenderingContext* context);

private:
	Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

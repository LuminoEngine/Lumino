
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
    void setLightDirection(const Vector3& value);
	void render(RenderingContext* context, const RenderViewPoint* viewPoint);

private:
	Ref<Material> m_material;
    Vector3 m_lightDirection;
};

} // namespace detail
} // namespace ln

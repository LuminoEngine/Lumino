
#include "Internal.hpp"
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoEngine/PostEffect/PostEffect.hpp>

namespace ln {

//==============================================================================
// PostEffect

PostEffect::PostEffect()
{
}

PostEffect::~PostEffect()
{
}

void PostEffect::init()
{
    Object::init();
}

void PostEffect::onDispose(bool explicitDisposing)
{
    //for (auto& i : m_instances) {
    //    i->m_ownerDisposed = true;
    //}

    Object::onDispose(explicitDisposing);
}

Ref<PostEffectInstance> PostEffect::createInstance()
{
    auto i = onCreateInstance();
    i->m_owner = this;
    //m_instances.add(i);
    return i;
}

void PostEffect::updateFrame(float elapsedSeconds)
{
    onUpdateFrame(elapsedSeconds);
}

void PostEffect::onUpdateFrame(float elapsedSeconds)
{
}

//void PostEffect::onPostRender()
//{
//}

} // namespace ln


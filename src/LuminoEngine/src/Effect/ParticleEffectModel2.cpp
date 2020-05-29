/*
 [2020/5/29] New ver 設計
 ----------

 モジュール分けを意識していきたい。今後も変化、というか機能追加ありそう。

 ### ParticleStorage
 個々のパーティクルの姿勢などの情報やライフタイム、生成と、そのデータのやりくりをするリングバッファ的なものを管理するもの。

 ### Modules
 Unity や UE4 のモジュール。パーティクルに対して何か変化を与えたいときはこれを Model に追加していく。

 ### Renderer
 Shape の Module ごとに存在。

 ### 関係

 - ParticleEffectModel
 - ParticleEffectEmitter
 この 2 つが 1:1 で、ユーザープログラムに公開するもののメイン。

 ParticleEffectEmitter
 - 最低1つの ParticleEmitterInstance を持つ。Model が Sub-emitter の定義を持っていれば、ParticleEmitterInstance は必要に応じてたくさんつくられる。
 - ShapeModule の key ごとに ParticleRenderer を持つ。
 
 ParticleModel と ParticleModelInstance が 1:1。それぞれ、ParticleEffectModel と ParticleEmitterInstance が複数持つ。



*/
#include "Internal.hpp"
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>
#include "ParticleEffectRenderer.hpp"

namespace ln {

//==============================================================================
// ParticleGeometry

ParticleGeometry::ParticleGeometry(ParticleGeometryType type)
    : m_type(type)
{
}

bool ParticleGeometry::init()
{
    if (!Object::init()) return false;
    return true;
}

//==============================================================================
// SpriteParticleGeometry

SpriteParticleGeometry::SpriteParticleGeometry()
    : ParticleGeometry(ParticleGeometryType::Sprite)
{
}

bool SpriteParticleGeometry::init()
{
    if (!Object::init()) return false;

    m_material = makeObject<Material>();
    m_material->setMainTexture(Texture2D::load(u"Sprite1"));
    m_material->shadingModel = ShadingModel::Unlit;
    m_material->setShader(Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));

    return true;
}

void SpriteParticleGeometry::setMaterial(Material* material)
{
    m_material = material;
}

uint64_t SpriteParticleGeometry::calculateRendererHashKey() const
{
    return reinterpret_cast<uint64_t>(m_material.get());
}

//==============================================================================
// ParticleEmitterModel2

ParticleEmitterModel2::ParticleEmitterModel2()
{
}

bool ParticleEmitterModel2::init()
{
    if (!Object::init()) return false;

    auto geom = makeObject<SpriteParticleGeometry>();
    m_geometry = geom;

    return true;
}

//==============================================================================
// ParticleModel2

ParticleModel2::ParticleModel2()
{
}

bool ParticleModel2::init()
{
    if (!Object::init()) return false;

    auto emitter = makeObject<ParticleEmitterModel2>();
    m_emitters.add(emitter);

    return true;
}

} // namespace ln

/*


 [2020/5/30] Orbit と Newtondynamics
 ----------
 Lumino 従来の Orbit(Rotation) は、角速度を利用した竜巻や放射の表現にうまく利用できていた。

 UE4 とかだと Orbit という名前らしい。

 ただ、改めて整理してみると、Orbit と 通常の速度・加速度 (LinearVelocity) を合わせて使うのってどういうときだろう？
 → Landscape プロジェクトの蘇生エフェクト

 同時使用はあり得るが、じゃあどちらを優先するべきだろう？

 Linear -> Orbit かなぁ。UE4 と同じ。
 まずは Linear で、個々のパーティクルのローカル姿勢を決める。その中で、Orbit を表現する。普通の Transform と同じ。
 逆の場合はちょっとパーティクル作るときのイメージがつかみにくくなるかも。恒星(Emitter位置)と惑星(Particle位置)で考えるみたいになる。

 Linear -> Orbit で
 - 竜巻を作るなら: Linear は速度Yは上昇。それ以外 0。Orbit の offset と角速度でぐるぐる回す。
 - Landscape プロジェクトの蘇生エフェクト: 竜巻と同じだが、Y を下降にするだけ。



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

 ----------

 - 小さいサイズの粒子ほど、遠くに飛ばしたい
 - 小さいサイズの粒子ほど、早く動かしたい
 - 

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
    m_lifeTime = { 5.0f, 5.0f, ParticleRandomSource::Self };
    m_size = { 1.0f, 1.0f, ParticleRandomSource::Self };
    m_forwardScale = { 1.0f, 1.0f, ParticleRandomSource::Self };
    m_crossScale = { 1.0f, 1.0f, ParticleRandomSource::Self };
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

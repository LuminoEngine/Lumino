/*

 [2020/5/30] Trail の設計
 ----------

 UE4 はノードの数を入力パラメータとしているが、「長さ」と「品質」で指定したほうがいい気がする…？

 とはいっても、毎フレーム Node を作らないと、パーティクルが早く動いてるときに不自然に見えてしまう。
 個数の見積もりにはフレームレートを使うことになるだろう。

 レンダリングは RibbonRenderer に任せる。
 角度が急になった時のアーティファクトを潰したりするのを担当する。ほかのモジュールでも使うかもしれないし、
 純粋にリボンを描くという機能を独立させておこう。

 RibbonRenderer独立はいいのだが、Node データは EmitterInstance 側で持っておかないとダメ。
 複数カメラから描画されたときに備えたい。Node は位置と太さを持って置き、
 カメラごとに呼ばれる onRender で、RibbonRenderer に書き込む。

 ### 長さ vs 個数

 長さを入力パラメータにするのは割と直感的でエフェクト作りやすいが、
 updateFrame ごとにひとつの Node を打つ方針だと、古い Node の位置を調整する必要が出てくる。
 処理落ちが起きて、一時的にスパンの長い Node ができた時。
 この方式だと、品質がフレームレートに依存しがちなのであんまりきれいに描画することはできないかもしれない…。

 打つ Node 数を、経過時間からちゃんと逆算してみたらどうだろう？
 今の ParticleEmitterInstance2::updateSpawn() みたいに。
 ↓
 この方式だと、例えば処理落ちしてて 1s 経ったとき、前回打った Node 位置から今回の位置まで、補間が必要になる。
 でも前回速度と今回速度はわかるし、cubic 補完が使える。
 今回 -> 前回へ向かって Node を打ちつつ距離を測って、指定長さ以上の Node は削除すればいい。
 実装難易度高いけど、エフェクトデザインはシンプルになるし、品質も良くなる。こっちの方式にしてみよう。

 ### UV座標の作り方
 2通りある。
 - Ribbon の先頭から末尾までで 0~1 とする
 - 指定長さで切って 0~1 を繰り返す

 最初のはいいとして、2つ目はちょっと面倒…というより、1 になるところで必ず Node を置く必要があるのですごく大変。
 代わりに、SamplerState を Repeat にすること前提で、長さに対して除算していくスタイルがいいかもしれない。

 ### 再考：長さでは見積もれない

 速度も必要になってしまう。「何秒残すか？」→「軌跡を引く時間」をパラメータとした方がいいかも


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
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Effect/ParticleEffectModel2.hpp>
#include "EffectManager.hpp"
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

    //m_material = makeObject_deprecated<Material>();
    //m_material->setMainTexture(Texture2D::load(_TT("Sprite1"));
    //m_material->shadingModel = ShadingModel::Unlit;
    //m_material->setShader(Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Sprite.fx"));

    return true;
}

void SpriteParticleGeometry::setMaterial(Material* material)
{
    m_material = material;
}

uint64_t SpriteParticleGeometry::calculateRendererHashKey(ParticleEmitterModel* emitterModel) const
{
    return reinterpret_cast<uint64_t>(m_material.get()) + static_cast<uint16_t>(emitterModel->m_geometryDirection);
}

//==============================================================================
// ParticleEmitterModel

LN_OBJECT_IMPLEMENT(ParticleEmitterModel, Object) {}

ParticleEmitterModel::ParticleEmitterModel()
{
    m_lifeTime = { 5.0f, 5.0f, ParticleRandomSource::Self };
    m_size = { 1.0f, 1.0f, ParticleRandomSource::Self };
    m_forwardScale = { 1.0f, 1.0f, ParticleRandomSource::Self };
    m_crossScale = { 1.0f, 1.0f, ParticleRandomSource::Self };
}

bool ParticleEmitterModel::init()
{
    if (!Object::init()) return false;

    //auto geom = makeObject_deprecated<SpriteParticleGeometry>();
    //m_geometry = geom;

    m_geometry = detail::EngineDomain::effectManager()->defaultSpriteParticleGeometry();

    return true;
}

void ParticleEmitterModel::serialize_deprecated(Serializer2_deprecated& ar)
{
    Object::serialize_deprecated(ar);
    ar & makeNVP(_TT("maxParticles"), m_spawnRate);
    ar & makeNVP(_TT("spawnRate"), m_maxParticles);
    ar & makeNVP(_TT("burstCount"), m_burstCount);
}

void ParticleEmitterModel::setupSpriteModule(Material* material)
{
    auto geom = makeObject_deprecated<SpriteParticleGeometry>();
    geom->setMaterial(material);
    m_geometry = geom;
}

void ParticleEmitterModel::setupBoxShape(const Vector3& size)
{
    m_shapeType = ln::ParticleEmitterShapeType::Box;
    m_shapeParam = size;
}

//==============================================================================
// ParticleModel

LN_OBJECT_IMPLEMENT(ParticleModel, Object) {}

ParticleModel::ParticleModel()
{
}

bool ParticleModel::init()
{
    if (!Object::init()) return false;

    //auto emitter = makeObject_deprecated<ParticleEmitterModel>();
    //m_emitters.add(emitter);

    return true;
}

void ParticleModel::addEmitter(ParticleEmitterModel* emitter)
{
    m_emitters.add(emitter);
}

void ParticleModel::serialize_deprecated(Serializer2_deprecated& ar)
{
    EffectResource::serialize_deprecated(ar);
    ar & makeNVP(_TT("emitters"), m_emitters);

}

} // namespace ln

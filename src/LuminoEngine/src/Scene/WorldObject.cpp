
#include "Internal.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// WorldObjectTransform

namespace detail {

WorldObjectTransform::WorldObjectTransform(WorldObject* parent)
    : m_parent(parent)
    , m_position(Vector3::Zero)
    , m_rotation(Quaternion::Identity)
    , m_scale(Vector3::Ones)
    //, m_worldMatrix()
{}

void WorldObjectTransform::setPosition(const Vector3& pos)
{
    if (m_position != pos)
    {
        m_position = pos;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::setRotation(const Quaternion& rot)
{
    if (m_rotation != rot)
    {
        m_rotation = rot;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::setScale(const Vector3& scale)
{
    if (m_scale != scale)
    {
        m_scale = scale;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::setCenterPoint(const Vector3& value)
{
    if (m_center != value)
    {
        m_center = value;
        m_parent->notifyTransformChanged();
    }
}

void WorldObjectTransform::lookAt(const Vector3& target, const Vector3& up)
{
    if (target == m_position) return;

    // left-hand coord
    Vector3 f = Vector3::normalize(target - m_position);


    Vector3 s = Vector3::cross(up, f);
    if (Vector3::nearEqual(s, Vector3::Zero))
    {
        // TODO: https://jp.mathworks.com/help/matlab/ref/circshift.html?requestedDomain=www.mathworks.com
        Vector3 u2 = Vector3::UnitZ;
        //std::rotate<float*>(&u2.x, (&u2.x)+2, &u2.z);
        s = Vector3::cross(u2, f);
    }

    s.normalize();

    Vector3 u = Vector3::cross(f, s);
    Matrix mat(
        s.x, s.y, s.z, 0.0f,
        u.x, u.y, u.z, 0.0f,
        f.x, f.y, f.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    m_rotation = Quaternion::makeFromRotationMatrix(mat);
}

Matrix WorldObjectTransform::getLocalMatrix() const
{
    return Matrix::makeAffineTransformation(m_scale, Vector3::Zero, m_rotation, m_position - m_center);
    //return Matrix::makeAffineTransformation(m_scale, m_center, m_rotation, m_position);
}

//void WorldObjectTransform::updateWorldMatrix(const Matrix* parent)
//{
//    Matrix localMatrix = Matrix::makeAffineTransformation(scale, center, rotation, position);
//
//    if (parent != nullptr)
//        m_worldMatrix = localMatrix * (*parent);
//    else
//        m_worldMatrix = localMatrix;
//}

} // namespace detail 

//==============================================================================
// WorldObject
//  Transform は Unity のように Component 扱いしない。
//  Component にしてしまうと、m_components リストを操作するときに混ざりこんでしまって危険かもしれない。
//  特に必要性が出てくるまではこのスタイルにしておく。

LN_OBJECT_IMPLEMENT(WorldObject, Object) {}

WorldObject::WorldObject()
    : m_scene(nullptr)
    , m_parent(nullptr)
    , m_transform(makeRef<detail::WorldObjectTransform>(this))
    , m_tags(makeList<String>())
    , m_components(makeList<Ref<Component>>())
    , m_children(makeList<Ref<WorldObject>>())
    , m_isSpecialObject(false)
	, m_destroyed(false)
{
}

WorldObject::~WorldObject()
{
}

void WorldObject::init()
{
    Object::init();

	World* activeWorld = detail::EngineDomain::sceneManager()->activeWorld();
	if (activeWorld) {
		activeWorld->addObject(this);
	}
}

void WorldObject::onDispose(bool explicitDisposing)
{
	m_destroyed = true;
	if (m_components) {
		m_components->clear();
	}
	Object::onDispose(explicitDisposing);
}

void WorldObject::lookAt(const Vector3& target, const Vector3& up)
{
    m_transform->lookAt(target, up);
}

void WorldObject::addComponent(Component* component)
{
    if (LN_REQUIRE(component)) return;
    if (LN_REQUIRE(!component->m_object)) return;

    component->m_object = this;
    m_components->add(component);
    component->onAttached(this);
}

void WorldObject::destroy()
{
	m_destroyed = true;
	if (m_scene) {
        m_scene->m_destroyList.add(this);
	}
}

void WorldObject::removeFromScene()
{
	if (m_scene) {
		if (m_parent) {
			LN_NOTIMPLEMENTED();
		}
		else {
            m_scene->removeRootObject(this);
		}
	}
}

const Matrix& WorldObject::worldMatrix()
{
    resolveWorldMatrix();
    return m_worldMatrix;
}

Component* WorldObject::findComponentByType(const TypeInfo* type) const
{
    return m_components->findIf([&](auto& x) { return TypeInfo::getTypeInfo(x) == type; }).valueOr(nullptr);
}

bool WorldObject::traverse(detail::IWorldObjectVisitor* visitor)
{
    if (!visitor->visit(this)) {
        return false;
    }
    for (auto& child : m_children) {
        if (!child->traverse(visitor)) {
            return false;
        }
    }
    return true;
}

void WorldObject::onPreUpdate()
{
}

void WorldObject::onUpdate(float elapsedSeconds)
{

}

void WorldObject::onRender()
{

}

bool WorldObject::traverseRefrection(ReflectionObjectVisitor* visitor)
{
	// 子 Component のプロパティをこのオブジェクトのものとして通知
	// TODO: ほんとは "Sprite.FrameIndex" みたいにオブジェクト名も入れた方がいいかも。
    // Unity だと、後からツリーを変更したときに、AnimationClip の path を修正するのがちょっと大変。
    // https://qiita.com/nekobako/items/b647a701b6070d1ca872
    // http://tsubakit1.hateblo.jp/entry/2018/02/08/203747
    // また、VMD や FBX から読み込んだアニメーションデータのトラックがボーン名しか持っていないときにも対応できるようにしておきたい。
    //
    // そうすると、
    // "FrameIndex" -> FrameIndex という名前のプロパティをすべて探して、最初に見つかったものを採用する。
    // "SpriteComponent.FrameIndex" -> Sprite の部分は型名またはオブジェクト名。この中のプロパティを探す。
    // ※ちなみに、SpriteComponent を直接使うことって少なくて、なのに "SpriteComponent" を明示的に指定しなければならないという仕様はちょっとイケてないと思う。
    //
    // あと、WPF でいうところの Logical, Visual ツリーの仕組みが必要かもしれない。
    // まぁ、Unity は持っているし。(ある 3DModel のボーンを示す子ノードの寿命は、親Nodeに完全に依存する)
    // このプロパティの検索は、ある Logical ノードの管理下にある Visual ツリーをたどることになる。
	for (auto& c : m_components) {
		if (TypeInfo* typeInfo = TypeInfo::getTypeInfo(c)) {
			for (auto& prop : typeInfo->properties()) {
				if (visitor->visitProperty(c, prop)) {
					return true;
				}
			}
		}
	}
	return false;
}

void WorldObject::serialize(Archive& ar)
{
	Object::serialize(ar);
	ar & ln::makeNVP(u"Components", *m_components);
	ar & ln::makeNVP(u"Children", *m_children);

    if (ar.isLoading()) {
        for (auto& c : m_components) {
            c->m_object = this;
            c->onAttached(this);
        }
    }
}

void WorldObject::attachScene(Scene* scene)
{
	if (LN_REQUIRE(scene)) return;
	if (LN_REQUIRE(!m_scene)) return;
	m_scene = scene;
	for (auto& c : m_components) {
		c->onAttachedScene(scene);
	}
}

void WorldObject::detachScene()
{
	if (m_scene) {
		Scene* old = m_scene;
		m_scene = nullptr;

		for (auto& c : m_components) {
			c->onDetachedScene(old);
		}
	}
}

void WorldObject::updateFrame(float elapsedSeconds)
{
    onUpdate(elapsedSeconds);
    for (auto& c : m_components) {
        c->onUpdate(elapsedSeconds);
    }
}

void WorldObject::render()
{
    onRender();
}

void WorldObject::notifyTransformChanged()
{
    if (!m_dirtyFlags.hasFlag(DirtyFlags::Transform))
    {
        m_dirtyFlags.set(DirtyFlags::Transform);

        for (auto& obj : m_children) {
            obj->notifyTransformChanged();
        }
    }
}

void WorldObject::resolveWorldMatrix()
{
    if (m_dirtyFlags.hasFlag(DirtyFlags::Transform)) {

        if (m_parent) {
            m_worldMatrix = m_transform->getLocalMatrix() * m_parent->worldMatrix();
        }
        else
            m_worldMatrix = m_transform->getLocalMatrix();

        m_dirtyFlags.unset(DirtyFlags::Transform);
    }
}

void WorldObject::updateWorldMatrixHierarchical()
{
    resolveWorldMatrix();

    for (auto& child : m_children)
    {
        child->updateWorldMatrixHierarchical();
    }
}


//==============================================================================
// WorldObjectAsset

namespace ed {

LN_OBJECT_IMPLEMENT(WorldObjectAsset, AssetModel) {}

WorldObjectAsset::WorldObjectAsset()
{
}

void WorldObjectAsset::init()
{
    AssetModel::init();
}

void WorldObjectAsset::serialize(Archive& ar)
{
	//ar & makeNVP(u"position", m_position);
	//ar & makeNVP(u"angles", m_angles);
	//ar & makeNVP(u"scale", m_scale);
}

} // namespace ed
} // namespace ln


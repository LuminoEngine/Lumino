
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Scene/Component.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Level.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// WorldObjectTransform
//  Note: UE4 の回転順序は      X(Roll) > Y(Pich) > Z(Yaw)
//  Note: three.js の回転順序は Z(Roll) > Y(Yaw) > X(Pich)
//  Note: Unity の回転順序は    Z(Roll) > X(Pich) > Y(Yaw)

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

// TODO: use Matrix::lookAt
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

    s.mutatingNormalize();

    Vector3 u = Vector3::cross(f, s);
    Matrix mat(
        s.x, s.y, s.z, 0.0f,
        u.x, u.y, u.z, 0.0f,
        f.x, f.y, f.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
    m_rotation = Quaternion::makeFromRotationMatrix(mat);

    m_parent->notifyTransformChanged();
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
// WorldObject::BuilderDetails

WorldObject::BuilderDetails::BuilderDetails()
    : position()
    , rotation()
    , scale(1.0f, 1.0f, 1.0)
{
}

void WorldObject::BuilderDetails::apply(WorldObject* p) const
{
    //if (width) p->setWidth(*width);
    //if (height) p->setHeight(*height);
    //if (backgroundColor) p->setBackgroundColor(*backgroundColor);
    p->setPosition(position);
    p->setRotation(rotation);
    p->setScale(scale);
    for (auto& x : m_components) p->addComponent(x.get());
}

//==============================================================================
// WorldObject
//  Transform は Unity のように Component 扱いしない。
//  Component にしてしまうと、m_components リストを操作するときに混ざりこんでしまって危険かもしれない。
//  特に必要性が出てくるまではこのスタイルにしておく。

LN_OBJECT_IMPLEMENT(WorldObject, Object) {}

WorldObject::WorldObject()
    : m_parentLevel(nullptr)
    , m_parent(nullptr)
    , m_transform(makeRef<detail::WorldObjectTransform>(this))
    , m_tags(makeList<String>())
    , m_components(makeObject_deprecated<ComponentList>())
    , m_children(makeList<Ref<WorldObject>>())
    , m_isSpecialObject(false)
	, m_destroyed(false)
{
    if (detail::EngineDomain::sceneManager()->m_editorMode) {
        m_id = detail::EngineDomain::sceneManager()->getWorldObjectId();

    }
}

WorldObject::~WorldObject()
{
}

bool WorldObject::init(/*ObjectInitializeContext* context*/)
{
	if (!Object::init()) return false;

    //if (ObjectInitializeContext::Default->autoAdd && detail::EngineDomain::sceneManager()->autoAddingToActiveWorld) {
    //    World* activeWorld = detail::EngineDomain::sceneManager()->activeWorld();
    //    if (activeWorld) {
    //        activeWorld->add(this);
    //    }
    //}

    onInit();

	return true;
}

void WorldObject::onDispose(bool explicitDisposing)
{
	m_destroyed = true;
	if (m_components) {
		m_components->clear();
	}

    if (m_id > 0) {
        detail::EngineDomain::sceneManager()->releaseWorldObjectId(m_id);
        m_id = 0;
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

void WorldObject::removeComponent(Component* component)
{
    if (LN_REQUIRE(component)) return;
    if (LN_REQUIRE(component->m_object == this)) return;

    component->m_object = nullptr;
    if (m_components->remove(component)) {
        component->onAttached(this);
    }
}

void WorldObject::removeAllComponents()
{
    if (m_components) {
        m_components->clear();
    }
}

void WorldObject::destroy()
{
	m_destroyed = true;
}

void WorldObject::removeFromParent()
{
    if (m_parent) {
        LN_NOTIMPLEMENTED();
    }

    if (m_parentLevel) {
        m_parentLevel->removeRootObject(this);
    }
}

//void WorldObject::addToWorld()
//{
//	World* activeWorld = detail::EngineDomain::sceneManager()->activeWorld();
//	if (LN_REQUIRE(activeWorld)) return;
//	activeWorld->add(this);
//}

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

void WorldObject::addInto(World* world)
{
    World* activeWorld = (world) ? world : detail::EngineDomain::sceneManager()->activeWorld();
    if (activeWorld) {
        activeWorld->add(this);
    }
}

void WorldObject::onInit()
{
}

void WorldObject::onPreUpdate()
{
}

void WorldObject::onUpdate(float elapsedSeconds)
{

}

void WorldObject::onRender(RenderingContext* context)
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
	for (auto& c : *m_components) {
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

void WorldObject::serialize_deprecated(Serializer2_deprecated& ar)
{
    Object::serialize_deprecated(ar);
    ar& ln::makeNVP(_TT("name"), m_name);

    Vector3 eularAngles = m_transform->m_rotation.toEulerAngles();
	ar & ln::makeNVP(_TT("position"), m_transform->m_position);
	ar & ln::makeNVP(_TT("angles"), eularAngles);   // Unity は Quaternion だけど、こっちは手打ち想定なので人が見やすい表現にする
	ar & ln::makeNVP(_TT("scale"), m_transform->m_scale);

	ar & ln::makeNVP(_TT("components"), *m_components);
	ar & ln::makeNVP(_TT("children"), *m_children);

    if (ar.isLoading()) {
        m_transform->m_rotation = Quaternion::makeFromEulerAngles(eularAngles);

        for (auto& c : *m_components) {
            c->m_object = this;
            c->onAttached(this);
        }

        notifyTransformChanged();
    }
}

void WorldObject::start()
{
	for (auto& c : *m_components) {
		c->onStart();
	}
}

void WorldObject::updateFrame(float elapsedSeconds)
{
    onUpdate(elapsedSeconds);
    for (auto& c : *m_components) {
        c->onUpdate(elapsedSeconds);
    }
}

void WorldObject::render(RenderingContext* context)
{
    context->setObjectId(m_id);
    onRender(context);
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

} // namespace ed
} // namespace ln


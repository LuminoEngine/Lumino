
#pragma once
#include "Common.h"
#include <Lumino/Base/String.h>
#include <Lumino/BindingSupport.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Material.h>
#include <Lumino/Game/Component.h>
//#include "MME/MMESceneObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class RenderingContext;
class SceneNodeList;

enum class DepthPriority
{
	Background = -1000,
	Normal = 0,
	Foreground = 1000,
};

/// SceneNode
LN_CLASS()
class SceneNode
	: public Component
	//: public tr::ReflectionObject
	//, public IMMESceneObject
{
	LN_OBJECT();
public:

	/// ノード名の設定
	void setName(const String& name);

	/// ノード名の取得
	const String& getName() const { return m_name; }

	/// ワールド変換行列の設定
	//void setTransform(const AttitudeTransform& transform) { m_transform = transform; }

	/// ワールド変換行列の取得
	//const AttitudeTransform& getTransform() const { return m_transform; }

	///** 位置を設定します。*/
	//LN_METHOD(Property)
	//void setPosition(const Vector3& pos) { m_transform.translation = pos; }
	//void setPosition(float x, float y, float z = 0.0f) { m_transform.translation.set(x, y, z); }

	///** 位置を取得します。*/
	//LN_METHOD(Property)
	//const Vector3& getPosition() const { return m_transform.translation; }

	///// 回転の設定
	//void setRotation(const Quaternion& rot) { m_transform.rotation = rot; }
	//void setAngles(float x, float y, float z) { m_transform.rotation = Quaternion::makeFromEulerAngles(Vector3(x, y, z)); }

	///// 回転の取得
	//const Quaternion& getRotation() const { return m_transform.rotation; }

	///// 拡大率の設定
	//void setScale(const Vector3& scale) { m_transform.scale = scale; }
	//void setScale(float xyz) { m_transform.scale.set(xyz, xyz, xyz); }
	//void setScale(float x, float y, float z = 0.0f) { m_transform.scale.set(x, y, z); }

	///// 拡大率の取得
	//const Vector3& getScale() const { return m_transform.scale; }

	///// 原点の設定
	//void setCenter(const Vector3& center) { m_transformCenter = center; }

	//void setCenter(float x, float y, float z = 0.0f) { m_transformCenter.set(x, y, z); }

	///// 原点の取得
	//const Vector3& getCenter() const { return m_transformCenter; }

	/** 可視状態を設定します。false の場合、ノードの描画自体行われません。(default: true) */
	LN_METHOD(Property)
	void setVisible(bool visible) { m_isVisible = visible; }

	/** 可視状態を取得します。*/
	LN_METHOD(Property)
	bool isVisible() const { return m_isVisible; }

	///// 回転順序の設定
	//void setRotateOrder(RotationOrder order) { m_rotOrder = order; }

	///// 回転順序の取得
	//RotationOrder getRotateOrder() const { return m_rotOrder; }

	/// 優先度の設定 (高い方から先に描画される)
	void setDepthPriority(DepthPriority priority) { m_priority = priority; }

	/// 優先度の取得
	DepthPriority getDepthPriority() const { return m_priority; }

	/// ビルボード状態の設定
	void setBillboardType(BillboardType type) { m_billboardType = type; }

	/// ビルボード状態の取得
	BillboardType getBillboardType() const { return m_billboardType; }

	/// 自動更新の有効設定
	void setEnableAutoUpdate(bool flag) { m_isAutoUpdate = flag; }

	/// 自動更新の有効判定
	bool isEnableAutoUpdate() const { return m_isAutoUpdate; }

	/// 子ノードの追加 (WPF の StakPanel.Children みたいに Collection を返すのも良いけど、addChild() はよく使うのでユーティリティとして定義s手置くのが良いかも)
	void addChild(SceneNode* child);
	void removeChild(SceneNode* child);

	SceneNodeRenderingMode getRenderingMode() const { return m_renderingMode; }
	void setRenderingMode(SceneNodeRenderingMode mode) { m_renderingMode = mode; }

	SceneNodeList* getChildren() const { return m_children; }

public:

	/// ノード種別の取得
	virtual SceneNodeType getSceneNodeType() const { return SceneNodeType_BaseNode; }

	/// 1フレーム分の更新処理
	virtual void onUpdateFrame(float deltaTime) {}

	/// グローバル座標変換行列と各プロパティを階層的に更新する
	///		この処理は1フレーム内で開始時に1度だけ呼ばれる。
	///		座標変換行列を更新し、描画するべきノードであるかをフィルタリングする。
	virtual void updateFrameHierarchy(SceneNode* parent, float deltaTime);


	virtual void onRender2(RenderingContext* renderer);


	// MME の CONTROLOBJECT アノテーション関係
	//virtual const Matrix& getMatrix() const { return m_combinedGlobalMatrix; }
	//virtual float getOpacity() const { return 0.0f; }
	//virtual const Matrix& FindFrameMarix(const String& name) const { return Matrix::Identity; }	// TODO
	//virtual float FindMorphBlend(const String& name) const { return 0.0f; }	// TODO
	//virtual LightNodeList* GetAffectLightList() { return NULL; }

protected:

	// Component interface
	virtual void onUpdate() override;

LN_INTERNAL_ACCESS:
	SceneNode();
	virtual ~SceneNode();
	void initialize();

	// TODO: これらもいらない
	void setAutoRemove(bool enabled) { m_isAutoRemove = enabled; }
	bool isAutoRemove() const { return m_isAutoRemove; }
	//const Matrix& GetCombinedGlobalMatrix() const { return m_combinedGlobalMatrix; }


protected:
	SceneGraphManager*	m_manager;	// TODO: いらない
	String				m_name;
	//Matrix				m_localMatrix;
	//AttitudeTransform		m_transform;
	//Vector3				m_transformCenter;
	//RotationOrder		m_rotOrder;
	DepthPriority		m_priority;
	BillboardType		m_billboardType;
	SceneNodeRenderingMode	m_renderingMode;
	//bool				m_transformModified;	///< 座標変換行列の再計算が必要か
	bool				m_isAutoUpdate;
	bool				m_isAutoRemove;

	RefPtr<SceneNodeList>	m_children;
	SceneNode*			m_parentNode;

	//Matrix				m_combinedGlobalMatrix;

	friend class DrawingLayer;

	//friend class MMERenderingPass;	// TODO: VisualComponent に持ってくべきかも。ついでに friend も何とかしたい
	//detail::RenderingPassClientData	m_renderingPassClientDataList[detail::MaxRenderingPass];

	detail::BuiltinEffectData	m_builtinEffectData;

	bool				m_isVisible;
private:
};

class SceneNodeList
	: public ObjectList<SceneNode*>
{
	LN_OBJECT();
LN_INTERNAL_ACCESS:
	SceneNodeList();
	virtual ~SceneNodeList();
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

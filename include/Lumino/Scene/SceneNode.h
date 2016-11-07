
#pragma once
#include "Common.h"
#include <Lumino/Base/String.h>
#include <Lumino/BindingSupport.h>
#include <Lumino/Graphics/Shader.h>
//#include "MME/MMESceneObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN
class DrawList;
class SceneNodeList;

/// SceneNode
class SceneNode
	: public tr::ReflectionObject
	//, public IMMESceneObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/// ノード名の設定
	void SetName(const String& name);

	/// ノード名の取得
	const String& GetName() const { return m_name; }

	/// ワールド変換行列の設定
	void SetTransform(const Matrix& matrix) { m_localMatrix = matrix; m_transformModified = false; }

	/// ワールド変換行列の取得
	const Matrix& GgetMatrix() const { return m_localMatrix; }

	/// 位置の設定
	void SetPosition(const Vector3& pos) { m_transform.translation = pos; m_transformModified = true; }
	void SetPosition(float x, float y, float z = 0.0f) { m_transform.translation.Set(x, y, z); m_transformModified = true; }

	/// 位置の取得
	const Vector3& GetPosition() const { return m_transform.translation; }

	/// 回転の設定
	void SetRotation(const Quaternion& rot) { m_transform.rotation = rot; m_transformModified = true; }
	void SetAngles(float x, float y, float z) { m_transform.rotation = Quaternion::MakeFromEulerAngles(Vector3(x, y, z)); m_transformModified = true; }

	/// 回転の取得
	const Quaternion& GetRotation() const { return m_transform.rotation; }

	/// 拡大率の設定
	void SetScale(const Vector3& scale) { m_transform.scale = scale; m_transformModified = true; }
	void SetScale(float xyz) { m_transform.scale.Set(xyz, xyz, xyz); m_transformModified = true; }
	void SetScale(float x, float y, float z = 0.0f) { m_transform.scale.Set(x, y, z); m_transformModified = true; }

	/// 拡大率の取得
	const Vector3& GetScale() const { return m_transform.scale; }

	/// 原点の設定
	void SetCenter(const Vector3& center) { m_transformCenter = center; m_transformModified = true; }

	void SetCenter(float x, float y, float z = 0.0f) { m_transformCenter.Set(x, y, z); m_transformModified = true; }

	/// 原点の取得
	const Vector3& GetCenter() const { return m_transformCenter; }

	///// 可視状態の設定
	//void SetVisible(bool enabled) { m_isVisible = enabled; }

	///// 可視状態の判定
	//bool IsVisible() const { return m_isVisible; }

	/// 回転順序の設定
	void SetRotateOrder(RotationOrder order) { m_rotOrder = order; }

	/// 回転順序の取得
	RotationOrder GetRotateOrder() const { return m_rotOrder; }

	/// 優先度の設定 (高い方から先に描画される)
	void SetPriority(int priority) { m_priority = priority; }

	/// 優先度の取得
	int GetPriority() const { return m_priority; }

	/// ビルボード状態の設定
	void SetBillboardType(BillboardType type) { m_billboardType = type; }

	/// ビルボード状態の取得
	BillboardType GetBillboardType() const { return m_billboardType; }

	/// 自動更新の有効設定
	void SetEnableAutoUpdate(bool flag) { m_isAutoUpdate = flag; }

	/// 自動更新の有効判定
	bool IsEnableAutoUpdate() const { return m_isAutoUpdate; }

	/// 子ノードの追加 (WPF の StakPanel.Children みたいに Collection を返すのも良いけど、AddChild() はよく使うのでユーティリティとして定義s手置くのが良いかも)
	void AddChild(SceneNode* child);
	void RemoveChild(SceneNode* child);

	SceneGraph* GetOwnerSceneGraph() { return m_ownerSceneGraph; }

	SceneNodeRenderingMode GetRenderingMode() const { return m_renderingMode; }
	void SetRenderingMode(SceneNodeRenderingMode mode) { m_renderingMode = mode; }

	SceneNodeList* GetChildren() const { return m_children; }

public:

	/// ノード種別の取得
	virtual SceneNodeType GetSceneNodeType() const { return SceneNodeType_BaseNode; }

	/// 1フレーム分の更新処理
	virtual void OnUpdateFrame(float deltaTime) {}

	/// グローバル座標変換行列と各プロパティを階層的に更新する
	///		この処理は1フレーム内で開始時に1度だけ呼ばれる。
	///		座標変換行列を更新し、描画するべきノードであるかをフィルタリングする。
	virtual void UpdateFrameHierarchy(SceneNode* parent, float deltaTime);

	/// 視点に依るデータを階層的に更新する (ある視点から描画されるとき、最初に呼び出される)
	///		renderingNodeList	: この視点の中で実際に描画するものはこのリストに追加する (視錘台カリングなど)
	///		renderingLightList	: this 以下の Light はこのリストに追加する
	///		この処理は1フレーム内で、レイヤー単位の描画開始時に呼ばれる。
	virtual void UpdateViewFlustumHierarchy(Camera* camera, SceneNodeArray* renderingNodeList, LightNodeList* renderingLightList);

	/// このノードの近いライトを選択する
	///		UpdateViewFlustumHierarchy() で、今回の描画で必要なライトが求まる。
	///		そのリストを使って、このノードの描画に影響するライトをリストアップする。
	///		maxCount はシェーダが必要としているライトの最大数。renderingLightList の中から maxCount 個取りだせば良い。
	virtual void UpdateAffectLights(LightNodeList* renderingLightList, int maxCount) {}

	/// Z ソート用の比較関数 (距離と優先度でソート)
	static bool CmpZAndPrioritySort(const SceneNode* left, const SceneNode* right);




	void Render2(DrawList* renderer);
	virtual void OnRender2(DrawList* renderer);


	// MME の CONTROLOBJECT アノテーション関係
	virtual bool IsVisible() const { return false; }
	virtual const Matrix& GetMatrix() const { return m_combinedGlobalMatrix; }
	virtual float GetOpacity() const { return 0.0f; }
	virtual const Matrix& FindFrameMarix(const String& name) const { return Matrix::Identity; }	// TODO
	virtual float FindMorphBlend(const String& name) const { return 0.0f; }	// TODO
	virtual LightNodeList* GetAffectLightList() { return NULL; }

protected:
	virtual void OnOwnerSceneGraphChanged(SceneGraph* newOwner, SceneGraph* oldOwner);

LN_INTERNAL_ACCESS:
	SceneNode();
	virtual ~SceneNode();
	void Initialize(SceneGraph* owner);
	void SetAutoRemove(bool enabled) { m_isAutoRemove = enabled; }
	bool IsAutoRemove() const { return m_isAutoRemove; }
	const Matrix& GetCombinedGlobalMatrix() const { return m_combinedGlobalMatrix; }

private:
	void SetOwnerSceneGraph(SceneGraph* owner);

protected:
	SceneGraphManager*	m_manager;	// TODO: いらない
	SceneGraph*			m_ownerSceneGraph;
	String				m_name;
	Matrix				m_localMatrix;
	SQTTransform		m_transform;
	Vector3				m_transformCenter;
	RotationOrder		m_rotOrder;
	int					m_priority;
	BillboardType		m_billboardType;
	SceneNodeRenderingMode	m_renderingMode;
	//bool				m_isVisible;
	bool				m_transformModified;	///< 座標変換行列の再計算が必要か
	bool				m_isAutoUpdate;
	bool				m_isAutoRemove;

	RefPtr<SceneNodeList>	m_children;
	SceneNode*			m_parentNode;

	Matrix				m_combinedGlobalMatrix;

	friend class DrawingLayer;
	float				m_zDistance;			///< Z ソート用の距離値 (ソート自体が速度的にクリティカルになりそうなので、getter を仮想関数にはしたくない)

	//friend class MMERenderingPass;	// TODO: VisualNode に持ってくべきかも。ついでに friend も何とかしたい
	//detail::RenderingPassClientData	m_renderingPassClientDataList[detail::MaxRenderingPass];
};

class SceneNodeList
	: public ObjectList<SceneNode*>
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
LN_INTERNAL_ACCESS:
	SceneNodeList();
	virtual ~SceneNodeList();
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

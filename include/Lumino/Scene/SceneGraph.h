
#pragma once
#include "../BindingSupport.h"
#include "../Graphics/Texture.h"
#include "Common.h"
#include "Detail.h"

LN_NAMESPACE_BEGIN
class SceneGraphManager;	// TODO: detail
class DrawList;
class Camera;
class SceneNode;
class StaticMeshModel;

/**
	@brief
*/
class SceneGraph
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/// レイヤーリストの取得
	//LayerList* GetLayerList() { return &m_layerList; }

	/// 現在の時間を取得する (秒)
	double GetTime() const { return m_time; }

	/// 前回フレームからの経過時間を取得する (秒)
	float GetElapsedTime() const { return m_elapsedTime; }

	/// 描画
	//void Render(RenderingContext* context, Camera* camera);
	virtual void Render2(DrawList* renderer, Camera* camera);

	/// マウス移動イベントを通知する (ViewPane の左上を 0,0 とした座標を指定する)
	bool InjectMouseMove(int x, int y);

	/// マウスボタンイベントを通知する
	bool InjectMouseButtonDown(MouseButton button, int x, int y);

	/// マウスボタンイベントを通知する
	bool InjectMouseButtonUp(MouseButton button, int x, int y);

	bool InjectMouseWheel(int delta);

	SceneGraphManager* GetManager() const { return m_manager; }
	virtual void UpdateFrame(float deltaTime);
	virtual SceneNode* GetRootNode() = 0;
	virtual Camera* GetMainCamera() = 0;

protected:
	SceneGraph();
	virtual ~SceneGraph();
	void CreateCore(SceneGraphManager* manager);

LN_INTERNAL_ACCESS:
	//void AddNode(SceneNode* node) { m_allNodes.Add(node); }
	//void RemoveNode(SceneNode* node) { m_allNodes.Remove(node); }
	List<Camera*>* GetAllCameraList() { return &m_allCameraList; }
	detail::SceneGraphRenderingProfilerInterface& GetRenderingProfiler() { return m_renderingProfiler; }

private:

	/// マウスボタンの状態
	struct MouseState
	{
		PointI	position;			// 最後にボタンが押されたときのマウスの座標 (最終的にシェーダに渡すのは -1.0～1.0 だが、スクリーンのリサイズに備えて元の座標で持っておく)
		float	time = 0.0f;		// 最後にボタンが押されたときの時間（秒）
		bool	isDown = false;		// 現在ボタンが押されているか

		void ToVector4(const Size& viewSize, Vector4* v)
		{
			v->x = (2.0f * ((float)position.x) / viewSize.width) - 1.0f;
			v->y = (2.0f * ((float)position.y) / viewSize.height) - 1.0f;
			v->z = (isDown) ? 1.0f : 0.0f;
			v->w = time;
		}
	};

	SceneGraphManager*	m_manager;
	//LayerList			m_layerList;

	double				m_time;					///< 時間処理の開始通知からの経過時間 (秒)
	float				m_elapsedTime;			///< 前回フレームからの経過時間 (秒)
	List<Camera*>		m_allCameraList;
	SceneNodeArray		m_renderingNodeList;	// 視錘台カリング等を行った後の、実際に描画するべきノードのリスト
	LightNodeList		m_renderingLightList;	// 描画ルート以下のライト (他の描画空間にライティングの影響を与えないようにするため)

	MouseState			m_leftMouseState;		///< マウスの左ボタンの状態
	MouseState			m_rightMouseState;		///< マウスの右ボタンの状態
	MouseState			m_middleMouseState;		///< マウスの中ボタンの状態
	PointI				m_mousePosition;		///< マウスの現在位置

	detail::SceneGraphRenderingProfilerInterface	m_renderingProfiler;
};


/**
	@brief
*/
class SceneGraph2D
	: public SceneGraph
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	virtual void UpdateFrame(float elapsedTime);
	virtual SceneNode* GetRootNode() override { return m_defaultRoot; }
	virtual Camera* GetMainCamera() override { return m_defaultCamera; }
	//virtual List<RenderingPass*>* GetRenderingPasses() override { return &m_renderingPasses; }

public:
	SceneGraph2D();
	virtual ~SceneGraph2D();
	void CreateCore(SceneGraphManager* manager);

private:
	SceneNode*				m_defaultRoot;
	Camera*					m_defaultCamera;
	//List<RenderingPass*>	m_renderingPasses;
};

/**
	@brief
*/
class SceneGraph3D
	: public SceneGraph
{
public:
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
	LN_TR_PROPERTY2(bool, VisibleGridPlane);

	virtual void UpdateFrame(float elapsedTime);
	virtual SceneNode* GetRootNode() override { return m_defaultRoot; }
	virtual Camera* GetMainCamera() override { return m_defaultCamera; }
	//virtual List<RenderingPass*>* GetRenderingPasses() override { return &m_renderingPasses; }

public:
	SceneGraph3D();
	virtual ~SceneGraph3D();
	void CreateCore(SceneGraphManager* manager);
	virtual void Render2(DrawList* renderer, Camera* camera) override;

private:
	void CreateGridContents();
	void AdjustGridMesh(Camera* camera);

	SceneNode*				m_defaultRoot;
	Camera*					m_defaultCamera;
	RefPtr<Light>			m_defaultLight;

	RefPtr<StaticMeshModel>	m_gridPlane;
};

LN_NAMESPACE_END

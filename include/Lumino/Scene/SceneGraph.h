
#pragma once
#include "../BindingSupport.h"
#include "../Graphics/Texture.h"
#include "Layer.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

class LayerList
	: public RefObjectList
{
public:
	LN_REF_OBJECT_LIST_DECL(Layer);
	LayerList() {}
	virtual ~LayerList() {}
};

/**
	@brief
*/
class SceneGraph
	: public RefObject
{
public:
	static SceneGraph* GetDefault2DSceneGraph();

	static SceneGraph* GetDefault3DSceneGraph();

public:

	/// レイヤーリストの取得
	LayerList* GetLayerList() { return &m_layerList; }

	/// 現在の時間を取得する (秒)
	double GetTime() const { return m_time; }

	/// 前回フレームからの経過時間を取得する (秒)
	float GetElapsedTime() const { return m_elapsedTime; }

	/// 描画
	void Render(Texture* renderTarget, Camera* camera);

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
	virtual Array<RenderingPass*>* GetRenderingPasses() = 0;

protected:
	SceneGraph();
	virtual ~SceneGraph();
	void CreateCore(SceneGraphManager* manager);

LN_INTERNAL_ACCESS:
	//void AddNode(SceneNode* node) { m_allNodes.Add(node); }
	//void RemoveNode(SceneNode* node) { m_allNodes.Remove(node); }
	Array<Camera*>* GetAllCameraList() { return &m_allCameraList; }

private:

	/// マウスボタンの状態
	struct MouseState
	{
		Point	Position;	///< 最後にボタンが押されたときのマウスの座標 (最終的にシェーダに渡すのは -1.0～1.0 だが、スクリーンのリサイズに備えて元の座標で持っておく)
		float	Time;		///< 最後にボタンが押されたときの時間（秒）
		bool	IsDown;		///< 現在ボタンが押されているか

		void ToVector4(const SizeF& viewSize, Vector4* v) {
			v->x = (2.0f * ((float)Position.x) / viewSize.width) - 1.0f;
			v->y = (2.0f * ((float)Position.y) / viewSize.height) - 1.0f;
			v->z = (IsDown) ? 1.0f : 0.0f;
			v->w = Time;
		}
	};

	SceneGraphManager*	m_manager;
	LayerList			m_layerList;

	double				m_time;					///< 時間処理の開始通知からの経過時間 (秒)
	float				m_elapsedTime;			///< 前回フレームからの経過時間 (秒)
	//SceneNodeList		m_allNodes;
	Array<Camera*>		m_allCameraList;
	SceneNodeList		m_renderingNodeList;	// 視錘台カリング等を行った後の、実際に描画するべきノードのリスト
	LightNodeList		m_renderingLightList;	// 描画ルート以下のライト (他の描画空間にライティングの影響を与えないようにするため)

	MouseState			m_leftMouseState;		///< マウスの左ボタンの状態
	MouseState			m_rightMouseState;		///< マウスの右ボタンの状態
	MouseState			m_middleMouseState;		///< マウスの中ボタンの状態
	Point				m_mousePosition;		///< マウスの現在位置
};


/**
	@brief
*/
class Basic2DSceneGraph
	: public SceneGraph
{
public:

	virtual void UpdateFrame(float elapsedTime);
	virtual SceneNode* GetRootNode() override { return m_defaultRoot; }
	virtual Camera* GetMainCamera() override { return m_defaultCamera; }
	virtual Array<RenderingPass*>* GetRenderingPasses() override { return &m_renderingPasses; }

public:
	Basic2DSceneGraph();
	virtual ~Basic2DSceneGraph();
	void CreateCore(SceneGraphManager* manager);

private:
	SceneNode*				m_defaultRoot;
	Camera*					m_defaultCamera;
	Array<RenderingPass*>	m_renderingPasses;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

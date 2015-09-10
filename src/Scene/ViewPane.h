
#pragma once
#include <Lumino/BindingSupport.h>
#include <Lumino/Platform/EventArgs.h>
#include <Lumino/Graphics/Texture.h>
#include "Layer.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

class LayerList
	: public RefObjectList
{
public:
	LN_REF_OBJECT_LIST_DECL(Layer);
	LayerList() {}
	virtual ~LayerList() {}
};

/// ViewPane
class ViewPane
	: public RefObject
{
public:
	ViewPane(SceneGraphManager* manager);
	virtual ~ViewPane();

public:

	/// レイヤーリストの取得
	LayerList* GetLayerList() { return &m_layerList; }

	/// 描画
	void Render(Graphics::Texture* renderTarget);

	/// マウス移動イベントを通知する (ViewPane の左上を 0,0 とした座標を指定する)
	bool InjectMouseMove(int x, int y);

	/// マウスボタンイベントを通知する
	bool InjectMouseButtonDown(MouseButton button);

	/// マウスボタンイベントを通知する
	bool InjectMouseButtonUp(MouseButton button);

private:

	/// マウスボタンの状態
	struct MouseState
	{
		Point	Position;	///< 最後にボタンが押されたときのマウスの座標 (最終的にシェーダに渡すのは -1.0～1.0 だが、スクリーンのリサイズに備えて元の座標で持っておく)
		float	Time;		///< 最後にボタンが押されたときの時間（秒）
		bool	IsDown;		///< 現在ボタンが押されているか

		void ToVector4(const SizeF& viewSize, Vector4* v) {
			v->X = (2.0f * ((float)Position.X) / viewSize.Width) - 1.0f;
			v->Y = (2.0f * ((float)Position.Y) / viewSize.Height) - 1.0f;
			v->Z = (IsDown) ? 1.0f : 0.0f;
			v->W = Time;
		}
	};

	SceneGraphManager*	m_manager;
	LayerList			m_layerList;

	MouseState			m_leftMouseState;		///< マウスの左ボタンの状態
	MouseState			m_rightMouseState;		///< マウスの右ボタンの状態
	MouseState			m_middleMouseState;		///< マウスの中ボタンの状態
	Point				m_mousePosition;		///< マウスの現在位置
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino

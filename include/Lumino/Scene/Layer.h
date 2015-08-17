
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "Common.h"
#include "Layer.h"

namespace Lumino
{
namespace Scene
{

/**
	@brief		レイヤーの基本クラスです。
*/
class Layer
	: public RefObject
{
protected:
	Layer();
	virtual ~Layer();
};

/**
	@brief		シーングラフのノードツリーを描画するレイヤーです。
*/
class DrawingLayer
	: public Layer
{
public:

	/**
		@brief		このレイヤーが描画するノードを指定します。
		@details	このレイヤーには指定したノードと、全ての子ノードが描画されます。
	*/
	virtual void SetRenderingRootNode(SceneNode* node) = 0;

	/**
		@brief		このレイヤーが描画するノードを取得します。
	*/
	virtual SceneNode* GetRenderingRootNode() const = 0;

	/**
		@brief		このレイヤーの描画に使用する視点を設定します。
	*/
	virtual void SetCamera(Camera* camera) = 0;

	/**
		@brief		このレイヤーの描画に使用する視点を取得します。
	*/
	virtual Camera* GetCamera() const = 0;

protected:
	DrawingLayer();
	virtual ~DrawingLayer();
	class DrawingLayerImpl;
	DrawingLayerImpl*	m_impl;
};


} // namespace Scene
} // namespace Lumino

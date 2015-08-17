
#pragma once
#include <Lumino/Graphics/Texture.h>
#include "Common.h"
#include "Layer.h"

namespace Lumino
{
namespace Scene
{

/**
	@brief		シーングラフの最終描画先となるレンダリングターゲットと、シーングラフを描画するためのレイヤーを管理します。
	@details	ViewPane はシーングラフを描画するためのルート要素です。
*/
class ViewPane
	: public RefObject
{
public:

	/**
		@brief	ViewPane のインスタンスを作成します。
	*/
	ViewPane* Create(SceneGraphManager* manager);

public:

	/**
		@brief	レイヤーのリストを取得します。
	*/
	virtual LayerList* GetLayerList() const = 0;

	/**
		@brief	指定したレンダリングターゲットに、全てのレイヤーを描画します。
	*/
	virtual void Render(Graphics::Texture* renderTarget) = 0;

public:
	ViewPane();
	virtual ~ViewPane();
};

} // namespace Scene
} // namespace Lumino

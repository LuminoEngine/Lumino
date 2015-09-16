
#pragma once

#include <Lumino/Base/Nullable.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include "Internal.h"
#include "MME/MMEShader.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

class RenderingParams
{
public:
	RenderingPass*				Pass;
	Renderer*					Renderer;
	GeometryRenderer*			GeometryRenderer;
	Camera*						CurrentCamera;
	MMEShader*					Shader;				///< 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)
};


struct RenderingPriorityParams
{
	MMEShader*		Shader;
	bool			Hide;
};

/// MMD でいうところの MMDPass ("object" 等) と、オフスクリーンRTに相当する。
/// RenderingPass は複数のレイヤーで共有できる。機能を追加するとき、
/// メンバに一時データが必要になる際は1回のレンダリングで完結するように注意すること。
class RenderingPass
	: public RefObject
{
public:
	static const int MaxRenderingPass = Internal::MaxRenderingPass;		///< 1 つのシーングラフ内で作成できる RenderingPass の最大数

public:
	struct NodeData
	{
		RenderingPass*	OwnerPass;
		int				PriorityShaderIndex;
	};

public:
	/// passID は、このパスが具体的に何を描画するのかを指定する。
	/// 65536 以降は予約済みで、InternalRenderingPass 列挙型で表される。
	/// もしユーザーがカスタムのパスを用意したい場合はこの ID を独自定義し、SceneNode::Render() 等で
	/// カスタムの処理を実装することを想定している。
	/// ownerShader は OFFSCREENRENDERTARGET としてこのパスを作るとき、生成元のシェーダを指定する
	RenderingPass(SceneGraphManager* manager/*, int passID, MMEShader* ownerShader = NULL*/);
	virtual ~RenderingPass();

public:

	//int GetPassID() const { return m_passID; }

	MMEShader* GetDefaultShader() { return m_defaultShader; }

	/// このパスで優先的に使用されるシェーダを追加する
	void AddPriorityShader(const String& matchingNameKey, MMEShader* shader, bool isHide);

public:

	/// 指定したノードをこのパス上で描画する
	///		以前は VisualNode に Render() を実装していたが、その処理をこちらに持ってきた。
	///		目的はノード単位のレンダリングの流れをパス側でカスタマイズできるようにすること。
	///		例えば、基底の処理は汎用的 MME 用のパスとなっており、ゲーム向けにはややオーバースペック気味。
	///		もっとゲーム向けにスマート化したカスタムパスを実装するとき、オーバーライドする。
	virtual void RenderNode(RenderingParams& params, SceneNode* node);

	virtual void PostRender(RenderingParams& params) {}


protected:

	struct PriorityParamsEntry
	{
		String					MatchingNameKey;		///< このシェーダを割り当てるノード名 (ワイドカード有り)
		RenderingPriorityParams	Params;
		//RefPtr<MMEShader>	Shader;					///< 割り当てるシェーダ
	};

	typedef Array<PriorityParamsEntry>	PriorityParamsEntryList;

	friend class SceneGraphManager;
	SceneGraphManager*			m_manager;
	int							m_internalEntryID;		///< このパスに割り当てられるID (0 ～ MaxRenderingPass-1)
	RefPtr<Texture>				m_renderTarget;			///< このパスの描画先となるレンダリングターゲット (NULL可。MMDPass の "zplot" であれば Zバッファがターゲットになる)
	RefPtr<Texture>				m_depthBuffer;			///< このパスの描画先となる深度バッファ (NULL可)
	RefPtr<MMEShader>			m_defaultShader;		///< このパスの描画でデフォルトとして使用されるシェーダ (シェーダを持っていない VisualNode に対して使われる)
	PriorityParamsEntryList		m_priorityEntryList;	///< このパスで優先的に使用されるシェーダ (シェーダを持っている VisualNode に対しても強制的にこちらを使う)
	Nullable<ColorF>			m_clearColor;			///< パス開始時にレンダリングターゲットをクリアする色
	Nullable<float>				m_clearDepth;			///< パス開始時に深度バッファをクリアするZ値
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino

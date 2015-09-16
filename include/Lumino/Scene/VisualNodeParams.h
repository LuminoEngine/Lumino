
#pragma once
#include <Lumino/Graphics/Material.h>
#include <Lumino/Graphics/RenderState.h>
#include "SceneNode.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN
namespace Internal
{

/// サブセット単位の描画パラメータ
struct VisualNodeSubsetParams
{
    Material		Material;       ///< 描画オブジェクトのマテリアル
    float					Opacity;        ///< 不透明度 (0.0 ～ 1.0) (シェーダに乗算色を送るとき、そのα値に乗算される)
	ColorF		ColorScale;     ///< 乗算色
	ColorF		BlendColor;     ///< ブレンド色
	Tone			Tone;           ///< 色調
    Matrix					UVTransform;    ///< テクスチャ座標変換行列
	MMEShader*				SceneShader;

	/// 初期値
	VisualNodeSubsetParams()
		: Opacity(1.0f)
		, ColorScale(1.0f, 1.0f, 1.0f, 1.0f)
		, BlendColor(0.0f, 0.0f, 0.0f, 0.0f)
		, Tone(0.0f, 0.0f, 0.0f, 0.0f)
		, UVTransform(Matrix::Identity)
		, SceneShader(NULL)
    {}

	~VisualNodeSubsetParams();

	void Multiply(const VisualNodeSubsetParams& parent);
};

/// VisualNodeParams
///		TODO: このクラスは昔の、描画スレッドを Scene で管理していたころの名残。VisualNode に統合してもよい。
class VisualNodeParams
{
public:
	VisualNodeParams();
	~VisualNodeParams();

public:

	/// 作成
	void Create(int subsetCount);

	/// サブセット単位の描画パラメータの取得 (-1 を指定するとグローバルパラメータを取得する)
	VisualNodeSubsetParams& GetSubsetParams(int index);
	const VisualNodeSubsetParams& GetSubsetParams(int index) const;

	/// 実際に描画時に使う情報の更新
	///		parentParams が NULL の場合は親から受け継がない。
	///		また、parentParams は UpdateSubsetRenderParam() 済みであること。
	void UpdateSubsetRenderParam(const VisualNodeParams* parentParams);

	const VisualNodeSubsetParams& GetCombinedSubsetParams(int index) const { return m_combinedSubsetParams[index]; }

private:
	VisualNodeSubsetParams				m_globalParams;			///< 全てのサブセットに影響するパラメータ
	Array<VisualNodeSubsetParams>		m_subsetParams;			///< 各サブセットのパラメータ
	VisualNodeSubsetParams				m_combinedGlobalParams;	///< 結合済みの、全てのサブセットに影響するパラメータ
	Array<VisualNodeSubsetParams>		m_combinedSubsetParams;	///< 結合済みの、各サブセットのパラメータ (UpdateSubsetRenderParam() で計算される)


};

} // namespace Internal
LN_NAMESPACE_SCENE_END
} // namespace Lumino

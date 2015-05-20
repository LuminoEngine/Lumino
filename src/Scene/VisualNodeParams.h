
#pragma once

#include <Lumino/Graphics/Material.h>
#include <Lumino/Graphics/RenderState.h>
#include "MME/MMEShader.h"
#include "SceneNode.h"

namespace Lumino
{
namespace Scene
{
/// サブセット単位の描画パラメータ
struct VisualNodeSubsetParams
{
    Graphics::Material		Material;       ///< 描画オブジェクトのマテリアル
    float					Opacity;        ///< 不透明度 (0.0 ～ 1.0) (シェーダに乗算色を送るとき、そのα値に乗算される)
	Graphics::ColorF		ColorScale;     ///< 乗算色
	Graphics::ColorF		BlendColor;     ///< ブレンド色
	Graphics::Tone			Tone;           ///< 色調
    Matrix					UVTransform;    ///< テクスチャ座標変換行列
	RefPtr<MMEShader>		SceneShader;

	/// 初期値
	VisualNodeSubsetParams()
        : Opacity           (1.0f)
        , ColorScale        (1.0f, 1.0f, 1.0f, 1.0f) 
        , BlendColor        (0.0f, 0.0f, 0.0f, 0.0f)
        , Tone              (0.0f, 0.0f, 0.0f, 0.0f)
		, UVTransform		(Matrix::Identity)
		, SceneShader		()
    {}

	void Multiply(const VisualNodeSubsetParams& parent)
	{
		Opacity *= parent.Opacity;
		ColorScale.MultiplyClamp(parent.ColorScale);
		BlendColor.AddClamp(parent.BlendColor);
		Tone.AddClamp(parent.Tone);
		if (SceneShader.IsNull()) {
			SceneShader = parent.SceneShader;
		}
	}
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
	ArrayList<VisualNodeSubsetParams>	m_subsetParams;			///< 各サブセットのパラメータ
	VisualNodeSubsetParams				m_combinedGlobalParams;	///< 結合済みの、全てのサブセットに影響するパラメータ
	ArrayList<VisualNodeSubsetParams>	m_combinedSubsetParams;	///< 結合済みの、各サブセットのパラメータ (UpdateSubsetRenderParam() で計算される)


};

} // namespace Scene
} // namespace Lumino

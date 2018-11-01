#pragma once
#include <LuminoEngine/Rendering/RenderView.hpp>

namespace ln {
namespace detail {

// TODO: 0.6.0 時点で、UI の描画最適化 (更新のあったところだけ再描画) を計画している。
// これは UI ツリーのあるコンテナを、RenderTarget を持つ特別なコンテナとするフラグを持たせ、
// 更新がある場合だけその RenderTarget を再描画することで行おうとしている。
// その時の RenderTarget を表現するのがこのクラス。となる予定。
class UIRenderView
	: public RenderView
{
public:
    UIRenderView();
	void initialize();

private:
};

} // namespace detail
} // namespace ln


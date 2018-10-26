#pragma once

namespace ln {
namespace detail {

/*
 * 描画リストやポストエフェクトなど描画に関わる全てのパラメータを受け取り、
 * RenderView から覗くことができる 1 枚の絵を描き上げる。
 */
class RenderingPipeline
	: public RefObject
{
public:
};

class SceneRenderingPipeline
    : public RefObject
{
public:
};

class UIRenderingPipeline
    : public RefObject
{
public:
};

} // namespace detail
} // namespace ln


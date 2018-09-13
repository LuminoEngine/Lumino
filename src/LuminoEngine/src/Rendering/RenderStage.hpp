#pragma once

namespace ln {
namespace detail {

/*
 * RenderStage
 * 
 * Unity でいうところの CommandBuffer に相当する。
 * State と Command(DrawElement) を固めて持っておくイメージ。
 * 基本的にどんなタイミングでも、「RenderStage を描画」すれば同じジオメトリが表示される。
 * 
 * 
 * 
 * 
 */
class RenderStage
	: public RefObject
{
public:

private:
};

class RenderStageList
	: public RefObject
{
public:
	RenderStageList();
	

private:
};

} // namespace detail
} // namespace ln


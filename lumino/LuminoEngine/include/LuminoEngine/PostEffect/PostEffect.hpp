#pragma once
#include "Common.hpp"

namespace ln {
class RenderTargetTexture;
class RenderingContext;
class PostEffectInstance;
namespace detail { class PostEffectRenderer; }


class PostEffect
	: public Object
{
protected:
	virtual void onDispose(bool explicitDisposing) override;

	Ref<PostEffectInstance> createInstance();

    virtual void onUpdateFrame(float elapsedSeconds);
	virtual Ref<PostEffectInstance> onCreateInstance() = 0;
	//virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;
	//virtual void onPostRender();

LN_CONSTRUCT_ACCESS:
	PostEffect();
	virtual ~PostEffect();
	void init();
    void updateFrame(float elapsedSeconds);

private:
	//List<PostEffectInstance*> m_instances;

    friend class detail::PostEffectRenderer;
};

//using PostEffectInstanceRenderer = PostEffect;

class PostEffectInstance
	: public Object
{
protected:
	// 周辺状況やパラメータ的に描画の必要がなかった場合は false を返す。
	virtual bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

private:
	//PostEffect* m_owner = nullptr;
	Ref<PostEffect> m_owner;
	bool m_ownerDisposed = false;

	friend class PostEffect;
	friend class detail::PostEffectRenderer;
};

} // namespace ln


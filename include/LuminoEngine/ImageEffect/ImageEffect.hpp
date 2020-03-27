#pragma once

namespace ln {
class RenderTargetTexture;
class RenderingContext;
class ImageEffectInstance;
namespace detail { class ImageEffectRenderer; }


class ImageEffect
	: public Object
{
protected:
	virtual void onDispose(bool explicitDisposing) override;

	Ref<ImageEffectInstance> createInstance();

    virtual void onUpdateFrame(float elapsedSeconds);
	virtual Ref<ImageEffectInstance> onCreateInstance() = 0;
	//virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;
	//virtual void onPostRender();

LN_CONSTRUCT_ACCESS:
	ImageEffect();
	virtual ~ImageEffect();
	void init();
    void updateFrame(float elapsedSeconds);

private:
	//List<ImageEffectInstance*> m_instances;

    friend class detail::ImageEffectRenderer;
};

//using ImageEffectInstanceRenderer = ImageEffect;

class ImageEffectInstance
	: public Object
{
protected:
	virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

private:
	//ImageEffect* m_owner = nullptr;
	Ref<ImageEffect> m_owner;
	bool m_ownerDisposed = false;

	friend class ImageEffect;
	friend class detail::ImageEffectRenderer;
};

} // namespace ln


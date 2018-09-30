
#pragma once
#include "GraphicsResource.hpp"
#include "GeometryStructs.hpp"

namespace ln {
class SamplerState;
class ShaderPass;
namespace detail { class ITexture; }

class Bitmap2D;

/** テクスチャのベースクラスです。 */
LN_CLASS()
class Texture
	: public GraphicsResource
{
public:
	/** テクスチャの幅を取得します。(ピクセル単位) */
	int width() const { return m_size.width; }

	/** テクスチャの高さを取得します。 (ピクセル単位) */
	int height() const { return m_size.height; }

	/** テクスチャのピフォーマットを取得します。 */
	TextureFormat format() const { return m_format; }

	/** このテクスチャに関連付けられている SamplerState を取得します。 */
	SamplerState* samplerState() const;

	/** このテクスチャに関連付ける SamplerState を設定します。 */
	void setSamplerState(SamplerState* value);

protected:
	Texture();
	virtual ~Texture();
	virtual void initialize();
	virtual detail::ITexture* resolveRHIObject() = 0;
	void setSize(const SizeI& size) { m_size = size; }
	void setFormat(TextureFormat format) { m_format = format; }

	SizeI m_size;
	TextureFormat m_format;
	Ref<SamplerState> m_samplerState;

	friend class ShaderPass;
};

/** 2D テクスチャのクラスです。 */
LN_CLASS()
class Texture2D
	: public Texture
{
public:

	/** テクスチャが保持するビットマップデータにアクセスします。 */
	Bitmap2D* map(MapMode mode);

	/** リソースの管理方法を変更します。(default: Managed) */
	void setResourcePool(GraphicsResourcePool pool);

LN_CONSTRUCT_ACCESS:
	Texture2D();
	virtual ~Texture2D();
	void initialize(int width, int height, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

protected:
	virtual void dispose() override;
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;
	virtual detail::ITexture* resolveRHIObject() override;

private:
	//void resizeInternal(int width, int height);

	Ref<detail::ITexture> m_rhiObject;
	GraphicsResourceUsage m_usage;
	GraphicsResourcePool m_pool;

	Ref<Bitmap2D> m_bitmap;
	void* m_rhiLockedBuffer;
	bool m_mipmap;
	bool m_initialUpdate;
	bool m_modified;
};

class RenderTargetTexture
	: public Texture
{
public:
	Ref<Bitmap2D> readData();

	virtual void dispose() override;

LN_CONSTRUCT_ACCESS:
	RenderTargetTexture();
	virtual ~RenderTargetTexture();
	void initialize(int width, int height, TextureFormat requestFormat, bool mipmap);
	void initialize(detail::ITexture* ref);

LN_INTERNAL_ACCESS:
	detail::ITexture* resolveRHIObject();
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	Ref<detail::ITexture> m_rhiObject;
	//SizeI m_size;
	//TextureFormat m_requestFormat;
	//bool m_mipmap;
};

} // namespace ln

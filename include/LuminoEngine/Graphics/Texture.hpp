#pragma once
#include "GraphicsResource.hpp"
#include "GeometryStructs.hpp"

namespace ln {
class SamplerState;
class ShaderPass;
namespace detail { class ITexture; }

class Bitmap2D;
class Bitmap3D;

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

	bool mipmap() const { return m_mipmap; }

	/** このテクスチャに関連付けられている SamplerState を取得します。 */
	SamplerState* samplerState() const;

	/** このテクスチャに関連付ける SamplerState を設定します。 */
	void setSamplerState(SamplerState* value);

protected:
	Texture();
	virtual ~Texture();
	void initialize();
	virtual detail::ITexture* resolveRHIObject() = 0;
	void setSize(const SizeI& size) { m_size = size; }
	void setFormat(TextureFormat format) { m_format = format; }
	void setMipmap(bool mipmap) { m_mipmap = mipmap; }

	SizeI m_size;
	TextureFormat m_format;
	Ref<SamplerState> m_samplerState;
	bool m_mipmap;

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
	void initialize(const StringRef& filePath, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

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
	bool m_initialUpdate;
	bool m_modified;
};


/** 3D テクスチャのクラスです。 */
LN_CLASS()
class Texture3D
	: public Texture
{
public:
	/** テクスチャの奥行きを取得します。 (ピクセル単位) */
	int depth() const { return m_depth; }

	/** テクスチャが保持するビットマップデータにアクセスします。 */
	Bitmap3D* map(MapMode mode);

	/** リソースの管理方法を変更します。(default: Managed) */
	void setResourcePool(GraphicsResourcePool pool);

LN_CONSTRUCT_ACCESS:
	Texture3D();
	virtual ~Texture3D();
	void initialize(int width, int height, int depth, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

protected:
	virtual void dispose() override;
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;
	virtual detail::ITexture* resolveRHIObject() override;

private:
	Ref<detail::ITexture> m_rhiObject;
	GraphicsResourceUsage m_usage;
	GraphicsResourcePool m_pool;

	int m_depth;
	Ref<Bitmap3D> m_bitmap;
	void* m_rhiLockedBuffer;
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
	virtual detail::ITexture* resolveRHIObject() override;
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	Ref<detail::ITexture> m_rhiObject;
	//SizeI m_size;
	//TextureFormat m_requestFormat;
	//bool m_mipmap;
};

namespace detail {

class TextureHelper
{
public:
	static void setMappedData(Texture2D* texture, const void* data);
};

} // namespace detail
} // namespace ln


#pragma once
#include "Common.h"
#include "Graphics.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail {
class RenderTargetTextureCache;
class IRenderFeature;

LN_ENUM_FLAGS(ContextStateFlags)
{
	None = 0x00,
	CommonState = 0x01,
	ShaderPass = 0x02,		// これ以外のステートに比べて割と頻繁に変更されるので個別に分ける
	All = 0xFFFF,
};
LN_ENUM_FLAGS_DECLARE(ContextStateFlags);

struct ContextState
{
	RenderState				renderState;
	DepthStencilState		depthStencilState;
	DepthBuffer*			depthBuffer = nullptr;
	RectI					viewport;

	Matrix					worldTransform;
	Matrix					viewTransform;
	Matrix					projectionTransform;


	ContextStateFlags		modifiedFlags = ContextStateFlags::None;

	ContextState();
	~ContextState();

	ContextState(const ContextState& obj) : ContextState() { copy(obj); }
	ContextState& operator=(const ContextState& obj) { copy(obj); return *this; }


	void setRenderTarget(int index, Texture* texture);
	Texture* getRenderTarget(int index) const;
	void setShaderPass(ShaderPass* pass);
	ShaderPass* getShaderPass() const { return m_shaderPass; }

	void setFillBrush(Brush* brush);
	Brush* getFillBrush() const { return m_fillBrush; }

	void reset()
	{
		renderState = RenderState();
		depthStencilState = DepthStencilState();
		depthBuffer = nullptr;
		viewport = RectI();
		//vertexBuffer = nullptr;
		//indexBuffer = nullptr;
	}

	void copy(const ContextState& obj);

private:
	std::array<Texture*, Graphics::MaxMultiRenderTargets>	m_renderTargets = {};
	Shader*		m_ownerShader = nullptr;
	ShaderPass*	m_shaderPass = nullptr;
	Brush*		m_fillBrush;
};


struct BasicContextState
{
	RenderState				renderState;
	DepthStencilState		depthStencilState;
	RefPtr<DepthBuffer>			depthBuffer;
	//RectI					viewport;

	void setRenderTarget(int index, Texture* texture);
	Texture* getRenderTarget(int index) const;
	void setShaderPass(ShaderPass* pass);
	ShaderPass* getShaderPass() const { return m_shaderPass; }

	bool equals(const BasicContextState& s) const;
	void copy(const BasicContextState& s);

private:
	std::array<RefPtr<Texture>, Graphics::MaxMultiRenderTargets>	m_renderTargets;
	RefPtr<Shader>			m_ownerShader;
	ShaderPass*				m_shaderPass = nullptr;
};



class ContextInterface
	: public RefObject
{
protected:
	ContextInterface();
	virtual ~ContextInterface();
	void initialize(GraphicsManager* manager);
	GraphicsManager* getManager() const { return m_manager; }

	void norityStateChanging();
	void norityStartDrawing(detail::IRenderFeature* rendererPloxy);
	void flushImplemented();

	// call by GraphicsManager
	void onActivated();
	void onDeactivated();

	virtual bool onCheckStateChanged() = 0;
	virtual void onStateFlush(detail::IRenderFeature* activeRenderer);
	virtual void onPrimitiveFlush();

	// call by ShaderVariable::setModified()
	virtual void onShaderVariableModified(ShaderVariable* var);

	void setBasicContextState(const BasicContextState& state);
	detail::CoreGraphicsRenderFeature* getBaseRenderer() const { return m_baseRenderer; }


public:
	// Utils
	static void makeBlendMode(BlendMode mode, RenderState* state);

private:
	void switchActiveRendererPloxy(detail::IRenderFeature* rendererPloxy);

	GraphicsManager*				m_manager;
	detail::CoreGraphicsRenderFeature*	m_baseRenderer;
	detail::IRenderFeature*			m_activeRendererPloxy;
	bool							m_stateChanged;

	friend class GraphicsManager;
	friend class ::ln::ShaderVariable;
};


template<class T>
class SimpleOneTimeObjectCache
	: public RefObject
{
public:
	T* queryCommandList()
	{
		MutexScopedLock lock(m_mutex);

		if (m_freeObjects.isEmpty())
		{
			auto ptr = createObject();
			m_objects.add(ptr);
			return ptr;
		}
		else
		{
			T* ptr;
			m_freeObjects.pop(&ptr);
			return ptr;
		}
	}

	void releaseCommandList(T* commandList)
	{
		MutexScopedLock lock(m_mutex);
		m_freeObjects.push(commandList);
	}

	void releaseAll()
	{
		MutexScopedLock lock(m_mutex);
		m_freeObjects.clear();
		for (RefPtr<T>& ptr : m_objects)
		{
			m_freeObjects.push(ptr);
		}
	}

protected:
	virtual RefPtr<T> createObject() = 0;

private:
	Mutex			m_mutex;
	List<RefPtr<T>>	m_objects;
	Stack<T*>		m_freeObjects;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END


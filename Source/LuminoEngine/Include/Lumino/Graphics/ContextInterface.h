
#pragma once
#include "Common.h"
#include "Graphics.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail {
class RenderTargetTextureCache;
class DrawElementBatch;

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

	ContextState(const ContextState& obj) : ContextState() { Copy(obj); }
	ContextState& operator=(const ContextState& obj) { Copy(obj); return *this; }


	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	void SetFillBrush(Brush* brush);
	Brush* GetFillBrush() const { return m_fillBrush; }

	void Reset()
	{
		renderState = RenderState();
		depthStencilState = DepthStencilState();
		depthBuffer = nullptr;
		viewport = RectI();
		//vertexBuffer = nullptr;
		//indexBuffer = nullptr;
	}

	void Copy(const ContextState& obj);

private:
	std::array<Texture*, Graphics::MaxMultiRenderTargets>	m_renderTargets = {};
	Shader*		m_ownerShader = nullptr;
	ShaderPass*	m_shaderPass = nullptr;
	Brush*		m_fillBrush;
};

class IRenderFeature
{
public:
	virtual bool IsStandaloneShader() const = 0;
	virtual void Flush() = 0;
	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;

	virtual void OnSetState(const DrawElementBatch* state) {}
};


struct BasicContextState
{
	RenderState				renderState;
	DepthStencilState		depthStencilState;
	RefPtr<DepthBuffer>			depthBuffer;
	//RectI					viewport;

	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	bool Equals(const BasicContextState& s) const;
	void Copy(const BasicContextState& s);

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
	void Initialize(GraphicsManager* manager);
	GraphicsManager* GetManager() const { return m_manager; }

	void NorityStateChanging();
	void NorityStartDrawing(detail::IRenderFeature* rendererPloxy);
	void FlushImplemented();

	// call by GraphicsManager
	void OnActivated();
	void OnDeactivated();

	virtual bool OnCheckStateChanged() = 0;
	virtual void OnStateFlush(detail::IRenderFeature* activeRenderer);
	virtual void OnPrimitiveFlush();

	// call by ShaderVariable::SetModified()
	virtual void OnShaderVariableModified(ShaderVariable* var);

	void SetBasicContextState(const BasicContextState& state);
	Details::Renderer* GetBaseRenderer() const { return m_baseRenderer; }


public:
	// Utils
	static void MakeBlendMode(BlendMode mode, RenderState* state);

private:
	void SwitchActiveRendererPloxy(detail::IRenderFeature* rendererPloxy);

	GraphicsManager*		m_manager;
	Details::Renderer*		m_baseRenderer;
	detail::IRenderFeature*	m_activeRendererPloxy;
	bool					m_stateChanged;

	friend class GraphicsManager;
	friend class ::ln::ShaderVariable;
};


template<class T>
class SimpleOneTimeObjectCache
	: public RefObject
{
public:
	T* QueryCommandList()
	{
		MutexScopedLock lock(m_mutex);

		if (m_freeObjects.IsEmpty())
		{
			auto ptr = CreateObject();
			m_objects.Add(ptr);
			return ptr;
		}
		else
		{
			T* ptr;
			m_freeObjects.Pop(&ptr);
			return ptr;
		}
	}

	void ReleaseCommandList(T* commandList)
	{
		MutexScopedLock lock(m_mutex);
		m_freeObjects.Push(commandList);
	}

	void ReleaseAll()
	{
		MutexScopedLock lock(m_mutex);
		m_freeObjects.Clear();
		for (RefPtr<T>& ptr : m_objects)
		{
			m_freeObjects.Push(ptr);
		}
	}

protected:
	virtual RefPtr<T> CreateObject() = 0;

private:
	Mutex			m_mutex;
	List<RefPtr<T>>	m_objects;
	Stack<T*>		m_freeObjects;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END


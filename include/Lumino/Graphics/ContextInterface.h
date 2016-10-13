
#pragma once
#include "Common.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail {
class RenderTargetTextureCache;

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


	static const int MaxMultiRenderTargets = 4;	// TODO

	RenderState				renderState;
	DepthStencilState		depthStencilState;
	Texture*				depthBuffer = nullptr;
	Rect					viewport;

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
		viewport = Rect();
		//vertexBuffer = nullptr;
		//indexBuffer = nullptr;
	}

	void Copy(const ContextState& obj);

private:
	std::array<Texture*, MaxMultiRenderTargets>	m_renderTargets = {};
	Shader*		m_ownerShader = nullptr;
	ShaderPass*	m_shaderPass = nullptr;
	Brush*		m_fillBrush;
};

class IRendererPloxy// : public RefObject
{
public:
	virtual void Flush() = 0;
	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
};


struct BasicContextState
{
	static const int MaxMultiRenderTargets = 4;	// TODO

	RenderState				renderState;
	DepthStencilState		depthStencilState;
	RefPtr<Texture>			depthBuffer;
	Rect					viewport;

	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	bool Equals(const BasicContextState& s) const;
	void Copy(const BasicContextState& s);

private:
	std::array<RefPtr<Texture>, MaxMultiRenderTargets>	m_renderTargets;
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
	void NorityStartDrawing(detail::IRendererPloxy* rendererPloxy);
	void FlushImplemented();

	// call by GraphicsManager
	void OnActivated();
	void OnDeactivated();

	virtual bool OnCheckStateChanged() = 0;
	virtual void OnStateFlush(detail::IRendererPloxy* activeRenderer);
	virtual void OnPrimitiveFlush();

	// call by ShaderVariable::SetModified()
	virtual void OnShaderVariableModified(ShaderVariable* var);

	void SetBasicContextState(const BasicContextState& state);
	Details::Renderer* GetBaseRenderer() const { return m_baseRenderer; }


	// Utils
	static void MakeBlendMode(BlendMode mode, RenderState* state);

private:
	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);

	GraphicsManager*		m_manager;
	Details::Renderer*		m_baseRenderer;
	detail::IRendererPloxy*	m_activeRendererPloxy;
	bool					m_stateChanged;

	friend class GraphicsManager;
	friend class ShaderVariable;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END


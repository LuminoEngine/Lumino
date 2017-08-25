
#pragma once 
#include "../ShaderVariableBase.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{
class DX9Renderer;
class DX9ShaderVariable;
class DX9ShaderTechnique;
class DX9ShaderPass;
class DX9ShaderAnnotation;

class DX9Shader
    : public IShader
{
public:
	static const char*		Macro_LN_HLSL_DX9;		///< "LN_HLSL_DX9"

	struct TextureVarInfo
	{
		DX9ShaderVariable*			variable;
		IDirect3DTexture9*			key;
		IDirect3DBaseTexture9*		originalTexture;
	};

public:
	DX9Shader(DX9GraphicsDevice* device, ID3DXEffect* dxEffect);
	virtual ~DX9Shader();

public:
	/// 作成
	static ShaderCompileResultLevel create(DX9GraphicsDevice* device, const char* code, size_t codeByteCount, DX9Shader** outShader, std::string* outMessage);

	DX9GraphicsDevice* getGraphicsDevice() { return m_device; }
	ID3DXEffect* getID3DXEffect() { return m_dxEffect; }

	//const SamplerState* FindSamplerState(IDirect3DBaseTexture9* dxTexture) const;
	List<TextureVarInfo>* getTextureVarInfoList() { return &m_textureVariables; }

	// override IShader
	virtual int getVariableCount() const { return m_variables.getCount(); }
	virtual IShaderVariable* getVariable(int index) const;
	virtual int getTechniqueCount() const { return m_techniques.getCount(); }
	virtual IShaderTechnique* getTechnique(int index) const;
	virtual void onLostDevice();
	virtual void onResetDevice();

private:

	DX9GraphicsDevice*				m_device;
	ID3DXEffect*					m_dxEffect;
	List<DX9ShaderVariable*>		m_variables;
	List<TextureVarInfo>			m_textureVariables;		// texture 型変数と sampler 型変数のペアを探すために使用する
	List<DX9ShaderTechnique*>		m_techniques;
};

/// シェーダ変数
class DX9ShaderVariable
	: public ShaderVariableBase
{
public:
	DX9ShaderVariable(DX9Shader* owner, D3DXHANDLE handle);
	virtual ~DX9ShaderVariable();

	D3DXHANDLE getHandle() const { return m_handle; }
	DX9TextureBase* getDX9TextureBase() const { return m_texture; }

	/// 初期値の読み取り用
	static void getValue(ID3DXEffect* dxEffect, D3DXHANDLE handle, ShaderVariableTypeDesc desc, ShaderValue* outValue);

public:
	virtual void setBool(bool value);
	virtual void setBoolArray(const bool* values, int count);
	virtual void setInt(int value);
	virtual void setFloat(float value);
	virtual void setFloatArray(const float* values, int count);
	virtual void setVector(const Vector4& vec);
	virtual void setVectorArray(const Vector4* vectors, int count);
	virtual void setMatrix(const Matrix& matrix);
	virtual void setMatrixArray(const Matrix* matrices, int count);
	virtual void setTexture(ITexture* texture);
	virtual int getAnnotationCount() { return m_annotations.getCount(); }
	virtual IShaderVariable* getAnnotation(int index);

private:
	ID3DXEffect*					m_dxEffect;
	D3DXHANDLE						m_handle;
	List<DX9ShaderAnnotation*>		m_annotations;
	List<BOOL>						m_temp;
	DX9TextureBase*					m_texture;
};

/// アノテーション
class DX9ShaderAnnotation
	: public DX9ShaderVariable
{
public:
	DX9ShaderAnnotation(DX9Shader* owner, D3DXHANDLE handle);
	virtual ~DX9ShaderAnnotation();

public:
	virtual void setBool(bool value) { LN_THROW(0, InvalidOperationException); }
	virtual void setInt(int value) { LN_THROW(0, InvalidOperationException); }
	virtual void setFloat(float value) { LN_THROW(0, InvalidOperationException); }
	virtual void setVector(const Vector4& vec) { LN_THROW(0, InvalidOperationException); }
	virtual void setVectorArray(const Vector4* vectors, int count) { LN_THROW(0, InvalidOperationException); }
	virtual void setMatrix(const Matrix& matrix) { LN_THROW(0, InvalidOperationException); }
	virtual void setMatrixArray(const Matrix* matrices, int count) { LN_THROW(0, InvalidOperationException); }
	virtual void setTexture(ITexture* texture) { LN_THROW(0, InvalidOperationException); }
	virtual int getAnnotationCount() { return 0; }
	virtual IShaderVariable* getAnnotation(int index) { return NULL; }
};

/// テクニック
class DX9ShaderTechnique
	: public IShaderTechnique
{
public:
	DX9ShaderTechnique(DX9Shader* owner, D3DXHANDLE handle);
	virtual ~DX9ShaderTechnique();

public:
	// override IShaderTechnique
	virtual const Char* getName() const { return m_name.c_str(); }
	virtual int getPassCount() const { return m_passes.getCount(); }
	virtual IShaderPass* getPass(int index);
	virtual int getAnnotationCount() { return m_annotations.getCount(); }
	virtual IShaderVariable* getAnnotation(int index) { return m_annotations[index]; }

private:
	ID3DXEffect*					m_dxEffect;
	D3DXHANDLE						m_handle;
	String							m_name;
	List<DX9ShaderAnnotation*>		m_annotations;
	List<DX9ShaderPass*>			m_passes;
};

/// パス
class DX9ShaderPass
	: public IShaderPass
{
public:
	DX9ShaderPass(DX9Shader* owner, D3DXHANDLE handle, int passIndex, D3DXHANDLE tech);
	virtual ~DX9ShaderPass();

public:
	// override IShaderPass
	virtual IShader* getShader() const { return m_owner; }
	virtual const Char* getName() const { return m_name.c_str(); }
	virtual int getAnnotationCount() { return m_annotations.getCount(); }
	virtual IShaderVariable* getAnnotation(int index) { return m_annotations[index]; }
	virtual void apply();

	void endPass();		// Renderer から呼ばれる
	void commitSamplerStatus();

private:
	struct SamplerLink
	{
		int					samplerIndex;
		DX9ShaderVariable*	variable;
	};

	DX9Shader*						m_owner;
	DX9Renderer*					m_renderer;
	ID3DXEffect*					m_dxEffect;
	D3DXHANDLE						m_handle;
	D3DXHANDLE						m_technique;
	int								m_passIndex;
	String							m_name;
	List<DX9ShaderAnnotation*>		m_annotations;
	List<SamplerLink>				m_samplerLinkList;
	bool							m_resolvedSamplerLink;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

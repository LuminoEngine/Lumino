
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

public:
	DX9Shader(DX9GraphicsDevice* device, ID3DXEffect* dxEffect);
	virtual ~DX9Shader();

public:
	/// 作成
	static ShaderCompileResultLevel Create(DX9GraphicsDevice* device, const char* code, size_t codeByteCount, DX9Shader** outShader, StringA* outMessage);

	DX9GraphicsDevice* GetGraphicsDevice() { return m_device; }
	ID3DXEffect* GetID3DXEffect() { return m_dxEffect; }

public:
	// override IShader
	virtual int GetVariableCount() const { return m_variables.GetCount(); }
	virtual IShaderVariable* GetVariable(int index) const;
	virtual int GetTechniqueCount() const { return m_techniques.GetCount(); }
	virtual IShaderTechnique* GetTechnique(int index) const;
	virtual void OnLostDevice();
	virtual void OnResetDevice();
	

private:
	DX9GraphicsDevice*				m_device;
	ID3DXEffect*					m_dxEffect;
	Array<DX9ShaderVariable*>		m_variables;
	Array<DX9ShaderTechnique*>		m_techniques;



	//DX9GraphicsDevice*		mGraphicsDevice;
 //   ID3DXEffect*            mDxEffect;
 //   LNShaderCompileResult   mCompileResult;
 //   lnRefString             mCompileErrors;
	//int						mVariableCount;			///< パラメータの数
 //   int						mTechniqueCount;		///< テクニックの数

	//DX9ShaderTechniqueArray	mShaderTechniqueArray;
 //   DX9ShaderTechniqueMap	mShaderTechniqueMap;
 //   DX9ShaderVariableArray	mShaderVariableArray;
 //   DX9ShaderVariableMap	mShaderVariableMap;
 //   DX9ShaderVariableArray	mTextureVariableArray;  ///< (サンプラステート実装のための検索対象)
};

/// シェーダ変数
class DX9ShaderVariable
	: public ShaderVariableBase
{
public:
	DX9ShaderVariable(DX9Shader* owner, D3DXHANDLE handle);
	virtual ~DX9ShaderVariable();

	/// 初期値の読み取り用
	static void GetValue(ID3DXEffect* dxEffect, D3DXHANDLE handle, ShaderVariableTypeDesc desc, ShaderValue* outValue);

public:
	virtual void SetBool(bool value);
	virtual void SetInt(int value);
	virtual void SetFloat(float value);
	virtual void SetVector(const Vector4& vec);
	virtual void SetVectorArray(const Vector4* vectors, int count);
	virtual void SetMatrix(const Matrix& matrix);
	virtual void SetMatrixArray(const Matrix* matrices, int count);
	virtual void SetTexture(ITexture* texture);
	virtual int GetAnnotationCount() { return m_annotations.GetCount(); }
	virtual IShaderVariable* GetAnnotation(int index);

private:
	ID3DXEffect*					m_dxEffect;
	D3DXHANDLE						m_handle;
	Array<DX9ShaderAnnotation*>		m_annotations;
};

/// アノテーション
class DX9ShaderAnnotation
	: public DX9ShaderVariable
{
public:
	DX9ShaderAnnotation(DX9Shader* owner, D3DXHANDLE handle);
	virtual ~DX9ShaderAnnotation();

public:
	virtual void SetBool(bool value) { LN_THROW(0, InvalidOperationException); }
	virtual void SetInt(int value) { LN_THROW(0, InvalidOperationException); }
	virtual void SetFloat(float value) { LN_THROW(0, InvalidOperationException); }
	virtual void SetVector(const Vector4& vec) { LN_THROW(0, InvalidOperationException); }
	virtual void SetVectorArray(const Vector4* vectors, int count) { LN_THROW(0, InvalidOperationException); }
	virtual void SetMatrix(const Matrix& matrix) { LN_THROW(0, InvalidOperationException); }
	virtual void SetMatrixArray(const Matrix* matrices, int count) { LN_THROW(0, InvalidOperationException); }
	virtual void SetTexture(ITexture* texture) { LN_THROW(0, InvalidOperationException); }
	virtual int GetAnnotationCount() { return 0; }
	virtual IShaderVariable* GetAnnotation(int index) { return NULL; }
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
	virtual const TCHAR* GetName() const { return m_name.GetCStr(); }
	virtual int GetPassCount() const { return m_passes.GetCount(); }
	virtual IShaderPass* GetPass(int index);
	virtual int GetAnnotationCount() { return m_annotations.GetCount(); }
	virtual IShaderVariable* GetAnnotation(int index) { return m_annotations[index]; }

private:
	ID3DXEffect*					m_dxEffect;
	D3DXHANDLE						m_handle;
	String							m_name;
	Array<DX9ShaderAnnotation*>		m_annotations;
	Array<DX9ShaderPass*>			m_passes;
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
	virtual const TCHAR* GetName() const { return m_name.GetCStr(); }
	virtual int GetAnnotationCount() { return m_annotations.GetCount(); }
	virtual IShaderVariable* GetAnnotation(int index) { return m_annotations[index]; }
	virtual void Apply();

	void EndPass();		// Renderer から呼ばれる

private:
	DX9Renderer*					m_renderer;
	ID3DXEffect*					m_dxEffect;
	D3DXHANDLE						m_handle;
	D3DXHANDLE						m_technique;
	int								m_passIndex;
	String							m_name;
	Array<DX9ShaderAnnotation*>		m_annotations;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

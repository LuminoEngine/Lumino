
#pragma once
#include "GraphicsDriverInterface.h"
#include <Lumino/Graphics/Shader.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

class ShaderVariableBase
    : public IShaderVariable
{
public:

	/// 型情報
	struct ShaderVariableTypeDesc
	{
		//ShaderVariableClass   Class;
		ShaderVariableType	Type;
		int					Rows;       ///< 行の数 (行列型以外は 1)
		int					Columns;    ///< 列の数 (ベクトル型の要素数でもある。GLSL にセットするときは API を区別するため、必要になる)
		int					Elements;   ///< 配列の要素数 (配列でない場合は 0)
		bool				Shared;     ///< shared キーワードがついている
	};

protected:
    ShaderVariableBase();
    virtual ~ShaderVariableBase();

protected:
	void initialize(ShaderVariableTypeDesc desc, const String& name, const String& semanticName);	// TODO: semanticName 消しとく
	void SetSemanticName(const String& semanticName) { m_semanticName = semanticName; }
	void MakeInitialValue();

public:
	// IShaderVariable
	virtual ShaderVariableType GetType() const override { return m_desc.Type; }
	virtual const String& GetName() const override { return m_name; }
	virtual const String& GetSemanticName() const override { return m_semanticName; }
	virtual int GetMatrixRows() const override { return m_desc.Rows; }
	virtual int GetMatrixColumns() const override { return m_desc.Columns; }
	virtual int GetArrayElements() const override { return m_desc.Elements; }
	virtual void SetBool(bool value) override;
	virtual void SetBoolArray(const bool* values, int count) override;
	virtual void SetInt(int value) override;
	virtual void SetFloat(float value) override;
	virtual void SetFloatArray(const float* values, int count) override;
	virtual void SetVector(const Vector4& vec) override;
	virtual void SetVectorArray(const Vector4* vectors, int count) override;
	virtual void SetMatrix(const Matrix& matrix) override;
	virtual void SetMatrixArray(const Matrix* matrices, int count) override;
	virtual void SetTexture(ITexture* texture) override;
	virtual const ShaderValue& GetValue() const override { return m_value; }

	void SetString(const char* str);
	void SetString(const String& str);
  
protected:
	ShaderVariableTypeDesc	m_desc;
	String					m_name;
	String					m_semanticName;
	ShaderValue	m_value;
	bool					m_modifid;
};

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

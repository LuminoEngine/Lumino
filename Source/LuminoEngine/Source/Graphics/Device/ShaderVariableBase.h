
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
	void setSemanticName(const String& semanticName) { m_semanticName = semanticName; }
	void makeInitialValue();

public:
	// IShaderVariable
	virtual ShaderVariableType getType() const override { return m_desc.Type; }
	virtual const String& getName() const override { return m_name; }
	virtual const String& getSemanticName() const override { return m_semanticName; }
	virtual int getMatrixRows() const override { return m_desc.Rows; }
	virtual int getMatrixColumns() const override { return m_desc.Columns; }
	virtual int getArrayElements() const override { return m_desc.Elements; }
	virtual void setBool(bool value) override;
	virtual void setBoolArray(const bool* values, int count) override;
	virtual void setInt(int value) override;
	virtual void setFloat(float value) override;
	virtual void setFloatArray(const float* values, int count) override;
	virtual void setVector(const Vector4& vec) override;
	virtual void setVectorArray(const Vector4* vectors, int count) override;
	virtual void setMatrix(const Matrix& matrix) override;
	virtual void setMatrixArray(const Matrix* matrices, int count) override;
	virtual void setTexture(ITexture* texture) override;
	virtual const ShaderValue& getValue() const override { return m_value; }

	void setString(const char* str);
	void setString(const String& str);
  
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

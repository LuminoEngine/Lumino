
#pragma once

#include "DeviceInterface.h"
#include <Lumino/Graphics/Shader.h>

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Device
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
		int					Elements;   ///< 配列の要素数 (配列でない場合は 0) TODO:OpenGLでは1…調査必要
		bool				Shared;     ///< shared キーワードがついている
	};

protected:
    ShaderVariableBase();
    virtual ~ShaderVariableBase();

protected:

	/// 初期化
	void Initialize(ShaderVariableTypeDesc desc, const String& name, const String& semanticName);

public:
	// IShaderVariable
	virtual ShaderVariableType GetType() const { return m_desc.Type; }
	virtual const String& GetName() const { return m_name; }
	virtual const String& GetSemanticName() const { return m_semanticName; }
	virtual int GetMatrixRows() const { return m_desc.Rows; }
	virtual int GetMatrixColumns() const { return m_desc.Columns; }
	virtual int GetArrayElements() const { return m_desc.Elements; }
	virtual void SetBool(bool value);
	virtual bool GetBool();
	virtual void SetInt(int value);
	virtual int GetInt();
	virtual void SetFloat(float value);
	virtual float GetFloat();
	virtual void SetVector(const Vector4& vec);
	virtual const Vector4& GetVector();
	virtual void SetVectorArray(const Vector4* vectors, int count);
	virtual const Vector4* GetVectorArray();
	virtual void SetMatrix(const Matrix& matrix);
	virtual const Matrix& GetMatrix();
	virtual void SetMatrixArray(const Matrix* matrices, int count);
	virtual const Matrix* GetMatrixArray();
	virtual void SetTexture(ITexture* texture);
	virtual ITexture* GetTexture();
	virtual const TCHAR* GetString();
	virtual const ShaderValue& GetValue() const { return m_value; }
	// IDeviceObject
	virtual void OnLostDevice();
	virtual void OnResetDevice();

	void SetString(const char* str);
  
protected:
	ShaderVariableTypeDesc	m_desc;
	String					m_name;
	String					m_semanticName;
	ShaderValue	m_value;
	bool					m_modifid;
};

} // namespace Device
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino

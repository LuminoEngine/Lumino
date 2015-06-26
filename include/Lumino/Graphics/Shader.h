
#pragma once

#include "Common.h"

namespace Lumino
{
namespace Graphics
{
class GraphicsManager;
class ShaderVariable;
class ShaderTechnique;
class ShaderPass;
class Texture;

/**
	@brief	シェーダのクラスです。
*/
class Shader
	: public RefObject
{
public:
	
	/**
		@brief		文字列をコンパイルし、シェーダを作成します。
		@param[in]	code			: シェーダコード文字列
		@param[in]	length			: 文字列の長さ (-1 で 終端 \0 まで)
		@details	テキストの文字コードは ASCII または UTF-8 (BOM無し) 推奨です。
					コンパイルエラーが発生した場合、例外 CompilationException が throw されます。
					(成功または警告のみの場合は throw されません)
					例外を throw せず、コンパイル結果の詳細を取得したいときは TryCreate() を使用してください。
	*/
	static Shader* Create(const char* code, int length = -1);

	/**
		@brief		メモリ上に展開されたテキストデータをコンパイルし、シェーダを作成します。
		@param[in]	manager			: 作成に使用する GraphicsManager
		@param[in]	textData		: シェーダコードが格納されているバッファアドレス
		@param[in]	byteCount		: textData のバイト数
		@details	テキストの文字コードは ASCII または UTF-8 (BOM無し) 推奨です。
					コンパイルエラーが発生した場合、例外 CompilationException が throw されます。
					(成功または警告のみの場合は throw されません)
					例外を throw せず、コンパイル結果の詳細を取得したいときは TryCreate() を使用してください。
	*/
	static Shader* Create(GraphicsManager* manager, const void* textData, size_t byteCount);
	
	/**
		@brief		メモリ上に展開されたテキストデータをコンパイルし、シェーダを作成します。
		@param[in]	manager			: 作成に使用する GraphicsManager
		@param[in]	textData		: シェーダコードが格納されているバッファアドレス
		@param[in]	byteCount		: textData のバイト数
		@param[in]	outShader		: 作成された Shader のポインタを格納する変数のアドレス
		@param[in]	outResult		: コンパイル結果の詳細を格納する変数のアドレス
		@return		コンパイルエラーがない場合は true (成功または警告のみの場合)
		@details	テキストの文字コードは ASCII または UTF-8 (BOM無し) 推奨です。
	*/
	static bool TryCreate(GraphicsManager* manager, const void* textData, size_t byteCount, Shader** outShader, ShaderCompileResult* outResult);

public:

	/**
		@brief		このシェーダで定義されている全ての変数を取得します。
	*/
	const Array<ShaderVariable*>& GetVariables() const { return m_variables; }
	
	/**
		@brief		名前を指定してシェーダ変数を検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderVariable* FindVariable(const TCHAR* name, CaseSensitivity cs = CaseSensitivity_CaseSensitive) const;

	/**
		@brief		このシェーダで定義されている全てのテクニックを取得します。
	*/
	const Array<ShaderTechnique*>& GetTechniques() const;

	GraphicsManager* GetManager() { return m_manager; }

protected:
	virtual ~Shader();

private:
	friend class RenderingCommandList;
	Shader(GraphicsManager* manager, Device::IShader* shader);
	GraphicsManager*			m_manager;
	Device::IShader*			m_deviceObj;
	Array<ShaderVariable*>		m_variables;
	Array<ShaderTechnique*>		m_techniques;
};



class ShaderValue
{
public:
	ShaderValue();
	~ShaderValue();
	ShaderValue(const ShaderValue& obj);
	ShaderValue& operator = (const ShaderValue& obj);

public:
	bool IsValid() const { return m_type != ShaderVariableType_Unknown; }

	void SetBool(bool value);
	bool GetBool() const { return m_value.BoolVal; }
	void SetInt(int value);
	int GetInt() const { return m_value.Int; }
	void SetFloat(float value);
	float GetFloat() const { return m_value.Float; }
	void SetVector(const Vector4& vec);
	const Vector4& GetVector() const { return *m_value.Vector; }
	void SetVectorArray(const Vector4* vectors, int count);
	const Vector4* GetVectorArray() const { return m_value.VectorArray; }
	void SetMatrix(const Matrix& matrix);
	const Matrix& GetMatrix() const { return *m_value.Matrix; }
	void SetMatrixArray(const Matrix* matrices, int count);
	const Matrix* GetMatrixArray() const { return m_value.MatrixArray; }
	void SetTexture(Device::ITexture* texture);
	Device::ITexture* GetTexture() const { return m_value.Texture; }
	void SetString(const char* str);
	const TCHAR* GetString() const { return m_value.String; }

	int GetArrayLength();
	byte_t* GetDataBuffer() { return m_value.Buffer; }	// 初期値格納用

private:

	struct Value
	{
		union
		{
			bool			BoolVal;
			int				Int;
			float			Float;
			Vector4*		Vector;
			Vector4*		VectorArray;
			Lumino::Matrix*	Matrix;
			Lumino::Matrix*	MatrixArray;
			Device::ITexture*		Texture;
			TCHAR*			String;

			byte_t*			Buffer;		// ↑のポインタ型の実態 (ITexture*以外)
		};
		//size_t ByteCount;
	};

	ByteBuffer			m_buffer;	// 他の ShaderValue と共有される
	ShaderVariableType	m_type;
	Value m_value;

	void ReleaseValueBuffer();
	void AllocValueBuffer(size_t byteCount);
	void Copy(const ShaderValue& value);

};

/**
	@brief	シェーダ変数のクラスです。
*/
class ShaderVariable
	: public RefObject
{
public:
	/// 変数の型の取得
	ShaderVariableType GetType() const;

	/// 変数名の取得
	const String& GetName() const;

	/// セマンティクス名の取得
	const String& GetSemanticName() const;

	/// 行列型の場合の行の数を取得する
	int GetRows() const;

	/// ベクトルまたは行列型の場合の列の数を取得する
	int GetColumns() const;

	/// 配列型の場合の要素数を取得する (0 の場合は配列ではない)
	int GetArrayElements() const;

	void SetBool(bool value);
	bool GetBool() const;
	void SetInt(int value);
	int GetInt() const;
	void SetFloat(float value);
	float GetFloat() const;
	void SetVector(const Vector4& value);
	const Vector4& GetVector() const;
	void SetVectorArray(const Vector4* values, int count);
	const Vector4* GetVectorArray() const;
	void SetMatrix(const Matrix& value);
	const Matrix& GetMatrix() const;
	void SetMatrixArray(const Matrix* values, int count);
	const Matrix* GetMatrixArray() const;
	void SetTexture(Texture* texture);
	Texture* GetTexture() const;
	void SetString(const char* str);
	const TCHAR* GetString() const;

	/**
		@brief		このシェーダ変数で定義されている全てのアノテーションを取得します。
	*/
	const Array<ShaderVariable*>& GetAnnotations() const;
	
private:
	friend class Shader;
	friend class ShaderTechnique;
	friend class ShaderPass;
	ShaderVariable(Shader* owner, Device::IShaderVariable* deviceObj);
	virtual ~ShaderVariable();
	Shader*						m_owner;
	Device::IShaderVariable*	m_deviceObj;
	ShaderValue					m_value;
	Texture*					m_textureValue;
	Array<ShaderVariable*>		m_annotations;
};

/**
	@brief	シェーダテクニックのクラスです。
*/
class ShaderTechnique
	: public RefObject
{
public:

	const Array<ShaderPass*>& GetPasses() const;
	

	/**
		@brief		このテクニックで定義されている全てのアノテーションを取得します。
	*/
	const Array<ShaderVariable*>& GetAnnotations() const;

	/**
		@brief		名前を指定してアノテーションを検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderVariable* FindAnnotation(const TCHAR* name, CaseSensitivity cs = CaseSensitivity_CaseSensitive) const;

protected:
	virtual ~ShaderTechnique();

private:
	friend class Shader;
	ShaderTechnique(Shader* owner, Device::IShaderTechnique* deviceObj);
	Shader*						m_owner;
	Device::IShaderTechnique*	m_deviceObj;
	Array<ShaderPass*>			m_passes;
	Array<ShaderVariable*>		m_annotations;
};

/**
	@brief	シェーダパスのクラスです。
*/
class ShaderPass
	: public RefObject
{
public:

	/**
		@brief	パスの名前を取得します。
	*/
	const String& GetName() const;

	/**
		@brief	パスを適用します。
	*/
	void Apply();

	/**
		@brief		このパスで定義されている全てのアノテーションを取得します。
	*/
	const Array<ShaderVariable*>& GetAnnotations() const;

	/**
		@brief		名前を指定してアノテーションを検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderVariable* FindAnnotation(const TCHAR* name, CaseSensitivity cs = CaseSensitivity_CaseSensitive) const;
	
protected:
	virtual ~ShaderPass();

private:
	friend class RenderingCommandList;
	friend class ShaderTechnique;
	ShaderPass(Shader* owner, Device::IShaderPass* deviceObj);

	Shader*						m_owner;
	Device::IShaderPass*		m_deviceObj;
	String						m_name;
	Array<ShaderVariable*>		m_annotations;
};

} // namespace Graphics
} // namespace Lumino

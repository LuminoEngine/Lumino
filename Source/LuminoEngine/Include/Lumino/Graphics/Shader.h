
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class ShaderVariable;
class Material;
class Shader;
using ShaderPtr = RefPtr<Shader>;

enum class ZSortDistanceBase
{
	NodeZ,					/**< ノードの Z 値を距離として使用する */
	CameraDistance,			/**< カメラとノードの距離を使用する */
	CameraScreenDistance,	/**< カメラが映すスクリーン平面とノードの距離を使用する */
};

namespace detail {
class CombinedMaterial;
class DynamicLightInfo;
	
// シェーダ変数セマンティクス
LN_ENUM(BuiltinSemantics)
{
	// Scene unit
	Dummy,

	// Camera unit
	View,
	Projection,
	ViewportPixelSize,

	// Element unit
	WorldViewProjection,
	World,
	LightEnables,			// bool[]
	LightWVPMatrices,		// matrix[]
	LightDirections,		// vector[]
	LightPositions,			// vector[]
	LightZFars,				// float[]
	LightDiffuses,			// vector[]
	LightAmbients,			// vector[]
	LightSpeculars,			// vector[]

	// Subset unit
	MaterialTexture,
	MaterialDiffuse,		// vector
	MaterialAmbient,		// vector
	MaterialEmmisive,		// vector
	MaterialSpecular,		// vector
	MaterialSpecularPower,	// float
	ColorScale,				// vector (Built-in effect)
	BlendColor,				// vector (Built-in effect)
	ToneColor,				// vector (Built-in effect)

	_Count,
};
LN_ENUM_DECLARE(BuiltinSemantics);

// シーン単位のデータに関する情報
struct SceneInfo
{
	//Matrix	ViewProjectionMatrix;
};

// カメラ単位のデータに関する情報
struct CameraInfo
{
	intptr_t	dataSourceId;
	Size		viewPixelSize;
	Vector3		viewPosition;
	Matrix		viewMatrix;
	Matrix		projMatrix;
	Matrix		viewProjMatrix;
	ViewFrustum	viewFrustum;
	ZSortDistanceBase	zSortDistanceBase;

	// POD
};

// 描画要素単位のデータに関する情報
struct ElementInfo
{
	const Matrix*		viewProjMatrix;

	Matrix				WorldMatrix;
	Matrix				WorldViewProjectionMatrix;
	DynamicLightInfo**	affectedLights;
};

// サブセット単位のデータに関する情報
struct SubsetInfo
{
	CombinedMaterial*	combinedMaterial;
	Texture*			materialTexture;		// TODO: これやめたい。Blit で使っているが・・・
};

// セマンティクスが関係するシェーダ変数の管理
class ShaderSemanticsManager
{
public:
	ShaderSemanticsManager();
	void initialize(GraphicsManager* manager);

	void TryPushVariable(ShaderVariable* var);
	void UpdateSceneVariables(const SceneInfo& info);
	void UpdateCameraVariables(const CameraInfo& info);
	void UpdateElementVariables(const ElementInfo& info);
	void UpdateSubsetVariables(const SubsetInfo& info);

	// Blit 用
	//void SetMaterialTexture(Texture* texture);

private:
	struct VariableKindPair
	{
		ShaderVariable*		variable;
		BuiltinSemantics	kind;
	};

	GraphicsManager*		m_manager;
	List<VariableKindPair>	m_sceneVariables;
	List<VariableKindPair>	m_cameraVariables;
	List<VariableKindPair>	m_elementVariables;
	List<VariableKindPair>	m_subsetVariables;
	intptr_t				m_lastCameraInfoId;

	MemoryStream			m_tempBuffer;
	BinaryWriter			m_tempBufferWriter;
};

} // namespace detail










/**
	@brief		シェーダのクラスです。
	@details	シェーダコードテキストの文字コードは ASCII または UTF-8 (BOM無し) 推奨です。
				コンパイルエラーが発生した場合、例外 CompilationException が throw されます。
				(成功または警告のみの場合は throw されません)
				例外を throw せず、コンパイル結果の詳細を取得したいときは TryCreate() を使用してください。
*/
class Shader
	: public GraphicsResourceObject
{
public:

	/**
		@brief		
	*/
	static ShaderPtr GetBuiltinShader(BuiltinShader shader);

	/**
		@brief		シェーダコードが記述されたテキストファイルをコンパイルし、Shader を作成します。
		@param[in]	filePath		: ファイルパス
	*/
	static RefPtr<Shader> create(const StringRef& filePath, bool useTRSS = false);

	/**
		@brief		メモリ上に展開されたテキストデータをコンパイルし、Shader を作成します。
		@param[in]	code			: シェーダコード文字列
		@param[in]	length			: 文字列の長さ (-1 で 終端 \0 まで)
	*/
	static RefPtr<Shader> create(const char* code, int length);
	
	///**
	//	@brief		文字列をコンパイルし、シェーダを作成します。
	//	@param[in]	code			: シェーダコード文字列
	//	@param[in]	length			: 文字列の長さ (-1 で 終端 \0 まで)
	//	@details	テキストの文字コードは ASCII または UTF-8 (BOM無し) 推奨です。
	//				コンパイルエラーが発生した場合、例外 CompilationException が throw されます。
	//				(成功または警告のみの場合は throw されません)
	//				例外を throw せず、コンパイル結果の詳細を取得したいときは TryCreate() を使用してください。
	//*/
	//static Shader* create(const char* code, int length = -1);

	///**
	//	@brief		メモリ上に展開されたテキストデータをコンパイルし、シェーダを作成します。
	//	@param[in]	manager			: 作成に使用する GraphicsManager
	//	@param[in]	textData		: シェーダコードが格納されているバッファアドレス
	//	@param[in]	byteCount		: textData のバイト数
	//	@details	テキストの文字コードは ASCII または UTF-8 (BOM無し) 推奨です。
	//				コンパイルエラーが発生した場合、例外 CompilationException が throw されます。
	//				(成功または警告のみの場合は throw されません)
	//				例外を throw せず、コンパイル結果の詳細を取得したいときは TryCreate() を使用してください。
	//*/
	//static Shader* create(GraphicsManager* manager, const void* textData, size_t byteCount);
	//
	///**
	//	@brief		メモリ上に展開されたテキストデータをコンパイルし、シェーダを作成します。
	//	@param[in]	manager			: 作成に使用する GraphicsManager
	//	@param[in]	textData		: シェーダコードが格納されているバッファアドレス
	//	@param[in]	byteCount		: textData のバイト数
	//	@param[in]	outShader		: 作成された Shader のポインタを格納する変数のアドレス
	//	@param[in]	outResult		: コンパイル結果の詳細を格納する変数のアドレス
	//	@return		コンパイルエラーがない場合は true (成功または警告のみの場合)
	//	@details	テキストの文字コードは ASCII または UTF-8 (BOM無し) 推奨です。
	//*/
	//static bool TryCreate(GraphicsManager* manager, const void* textData, size_t byteCount, Shader** outShader, ShaderCompileResult* outResult);

public:

	/**
		@brief		このシェーダで定義されている全ての変数を取得します。
	*/
	const List<ShaderVariable*>& GetVariables() const { return m_variables; }
	
	/**
		@brief		名前を指定してシェーダ変数を検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderVariable* FindVariable(const TCHAR* name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	ShaderVariable* FindVariable(const String& name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const { return FindVariable(name.c_str(), cs); }

	/**
		@brief		このシェーダで定義されている全てのテクニックを取得します。
	*/
	const List<ShaderTechnique*>& GetTechniques() const;

	/**
		@brief		名前を指定してテクニックを検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderTechnique* FindTechnique(const TCHAR* name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	ShaderTechnique* FindTechnique(const String& name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const { return FindTechnique(name.c_str(), cs); }

	detail::GraphicsManager* GetManager() { return m_manager; }

protected:
	virtual ~Shader();
	virtual void Dispose() override;
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

LN_INTERNAL_ACCESS:
	friend class RenderingCommandList;
	Shader();
	void initialize(detail::GraphicsManager* manager, const StringRef& filePath, bool useTRSS = false);
	void initialize(detail::GraphicsManager* manager, const void* code, int length, bool useTRSS = false);
	void PostInitialize();
	void SetModifiedVariables(bool modified) { m_modifiedVariables = modified; }
	bool IsModifiedVariables() const { return m_modifiedVariables; }
	void TryCommitChanges();
	detail::ShaderSemanticsManager* GetSemanticsManager() { return &m_semanticsManager; }
	void SetShaderValue(uint32_t variableNameHash, const ShaderValue& value);

	ByteBuffer						m_sourceCode;
	Driver::IShader*				m_deviceObj;
	List<ShaderVariable*>			m_variables;
	List<ShaderTechnique*>			m_techniques;
	detail::ShaderSemanticsManager	m_semanticsManager;
	bool							m_modifiedVariables;
};


// TODO: Driver でも使っている。初期値を保持するため。
// Material でも使っている。こちらは ManagedTexture を使う。
class ShaderValue
{
public:
	ShaderValue();
	~ShaderValue();
	ShaderValue(const ShaderValue& obj);
	ShaderValue& operator = (const ShaderValue& obj);

public:
	bool IsValid() const { return m_type != ShaderVariableType_Unknown; }
	ShaderVariableType getType() const { return m_type; }

	void SetBool(bool value);
	bool GetBool() const { return m_value.BoolVal; }
	void setInt(int value);
	int getInt() const { return m_value.Int; }
	void SetBoolArray(const bool* values, int count);
	const bool* GetBoolArray() const { return m_value.BoolArray; }
	void SetFloat(float value);
	float GetFloat() const { return m_value.Float; }
	void SetFloatArray(const float* values, int count);
	const float* GetFloatArray() const { return m_value.FloatArray; }
	void SetVector(const Vector4& vec);
	const Vector4& GetVector() const { return *m_value.Vector; }
	void SetVectorArray(const Vector4* vectors, int count);
	const Vector4* GetVectorArray() const { return m_value.VectorArray; }
	void SetMatrix(const Matrix& matrix);
	const Matrix& GetMatrix() const { return *m_value.Matrix; }
	void SetMatrixArray(const Matrix* matrices, int count);
	const Matrix* GetMatrixArray() const { return m_value.MatrixArray; }
	void SetDeviceTexture(Driver::ITexture* texture);
	Driver::ITexture* GetDeviceTexture() const { return m_value.DeviceTexture; }
	void SetManagedTexture(Texture* texture);
	Texture* GetManagedTexture() const { return m_value.ManagedTexture; }
	void setString(const char* str);
	void setString(const String& str);
	const TCHAR* getString() const { return m_value.String; }

	int GetArrayLength() const;
	byte_t* GetDataBuffer() { return m_value.Buffer; }	// 初期値格納用

	bool equals(const ShaderValue& value) const;
	uint32_t getHashCode();

private:

	static bool IsBufferCopyType(ShaderVariableType type)
	{
		return
			type == ShaderVariableType_BoolArray ||
			type == ShaderVariableType_FloatArray ||
			type == ShaderVariableType_Vector ||
			type == ShaderVariableType_VectorArray ||
			type == ShaderVariableType_Matrix ||
			type == ShaderVariableType_MatrixArray ||
			type == ShaderVariableType_String;
	}

	struct Value
	{
		union
		{
			bool			BoolVal;
			bool*			BoolArray;
			int				Int;
			float			Float;
			float*			FloatArray;
			Vector4*		Vector;
			Vector4*		VectorArray;
			ln::Matrix*	Matrix;
			ln::Matrix*	MatrixArray;
			Driver::ITexture*	DeviceTexture;
			Texture*			ManagedTexture;
			TCHAR*			String;

			byte_t*			Buffer;		// ↑のポインタ型の実態 (ITexture*以外)
		};
		//size_t ByteCount;
	};

	ByteBuffer			m_buffer;	// 他の ShaderValue と共有される
	ShaderVariableType	m_type;
	Value m_value;
	uint32_t			m_hashCode;
	bool				m_hashDirty;

	void ReleaseValueBuffer();
	void AllocValueBuffer(size_t byteCount);
	void copy(const ShaderValue& value);

};

/**
	@brief	シェーダ変数のクラスです。
*/
class ShaderVariable
	: public RefObject
{
public:
	/// 変数の型の取得
	ShaderVariableType getType() const;

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
	void SetBoolArray(const bool* values, int count);
	void setInt(int value);
	int getInt() const;
	void SetFloat(float value);
	float GetFloat() const;
	void SetFloatArray(const float* values, int count);
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
	void setString(const char* str);
	const TCHAR* getString() const;

	/**
		@brief		このシェーダ変数で定義されている全てのアノテーションを取得します。
	*/
	const List<ShaderVariable*>& GetAnnotations() const;

	/**
		@brief		名前を指定してアノテーションを検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderVariable* FindAnnotation(const TCHAR* name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	ShaderVariable* FindAnnotation(const String& name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const { return FindAnnotation(name.c_str(), cs); }
	
LN_INTERNAL_ACCESS:
	void ChangeDevice(Driver::IShaderVariable* obj);
	void SetModified();
	void OnCommitChanges();
	uint32_t GetNameHash() const { return m_nameHash; }
	Shader* GetOwnerShader() const { return m_owner; }
	Driver::IShaderVariable* GetDeviceObject() const { return m_deviceObj; }
	void SetShaderValue(const ShaderValue& value);
	const ShaderValue& GetShaderValue() const { return m_value; }

private:
	friend class Shader;
	friend class ShaderTechnique;
	friend class ShaderPass;
	ShaderVariable(Shader* owner, Driver::IShaderVariable* deviceObj);
	virtual ~ShaderVariable();
	Shader*						m_owner;
	String						m_name;
	Driver::IShaderVariable*	m_deviceObj;
	uint32_t					m_nameHash;
	ShaderValue					m_value;
	Texture*					m_textureValue;		// ShaderValue は Driver でも使われるので ITexture* を持ち、Texture* は持たない。 
	List<ShaderVariable*>		m_annotations;
	bool						m_modified;
};

/**
	@brief	シェーダテクニックのクラスです。
*/
class ShaderTechnique
	: public RefObject
{
public:

	const String& GetName() const;

	const List<ShaderPass*>& GetPasses() const;
	
	/**
		@brief		名前を指定してパスを検索します。
		@exception	KeyNotFoundException
	*/
	ShaderPass* GetPass(const TCHAR* name) const;

	/**
		@brief		このテクニックで定義されている全てのアノテーションを取得します。
	*/
	const List<ShaderVariable*>& GetAnnotations() const;

	/**
		@brief		名前を指定してアノテーションを検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderVariable* FindAnnotation(const TCHAR* name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	ShaderVariable* FindAnnotation(const String& name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const { return FindAnnotation(name.c_str(), cs); }

protected:
	virtual ~ShaderTechnique();

LN_INTERNAL_ACCESS:
	void ChangeDevice(Driver::IShaderTechnique* obj);

private:
	friend class Shader;
	ShaderTechnique(Shader* owner, Driver::IShaderTechnique* deviceObj);
	Shader*						m_owner;
	Driver::IShaderTechnique*	m_deviceObj;
	String						m_name;
	List<ShaderPass*>			m_passes;
	List<ShaderVariable*>		m_annotations;
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
		@brief		このパスで定義されている全てのアノテーションを取得します。
	*/
	const List<ShaderVariable*>& GetAnnotations() const;

	/**
		@brief		名前を指定してアノテーションを検索します。
		@return		見つからなかった場合は NULL を返します。
	*/
	ShaderVariable* FindAnnotation(const TCHAR* name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	ShaderVariable* FindAnnotation(const String& name, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const { return FindAnnotation(name.c_str(), cs); }
	
private:
	friend class Details::Renderer;
	void Apply();

protected:
	virtual ~ShaderPass();

LN_INTERNAL_ACCESS:
	void ChangeDevice(Driver::IShaderPass* obj);
	Shader* GetOwnerShader() { return m_owner; }

private:
	friend class RenderingCommandList;
	friend class ShaderTechnique;
	ShaderPass(Shader* owner, Driver::IShaderPass* deviceObj);

	Shader*						m_owner;
	Driver::IShaderPass*		m_deviceObj;
	String						m_name;
	List<ShaderVariable*>		m_annotations;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

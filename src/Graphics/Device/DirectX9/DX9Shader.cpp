/*
	[13/12/22] メモリバッファからの読み込みはサポートしない
		今のところ以下のような問題がある。
		・#include 対応のためのカレントディレクトリの管理は誰がやるの？
		・OpenGL ES の時、デバイスロストしたときの再構築に、シェーダソースが必要。
		  ずっとメンバに持っててもいいけど、メモリが無駄になる。
		特に後者は重要で、テクスチャも同じ扱いにする必要がありそう。
			…でも、そうなると頂点バッファ・インデックスバッファもデータ持っておく必要がある…。
			→やっぱり、メモリデータごとバックアップ持っておいた方が良いかもしれない。
			  マルチスレッド完璧に対応するにはダブルバッファ的なものが必要になって来るし…。

		⇒とりあえず本件については、カレントディレクトリに限らず.lnfx の読み方をどうするとか
		  ファイル名だけの方が都合がいいいので、今のところサポートしない。
		  エラーもわかりやすいのが出せるはず。
		  要望あれば警告したうえで実装する。

	[14/2/12] やっぱりメモリバッファから読みたい
		それにその方が自然な気がする…。
		それに、Android とかフレームワークに則って作るなら、
		onResume() とかで onPause()時点の状態に復帰させる のはプログラマの責任。
		でも、onStop() は完全にメモリ上から削除されるときだからこれは使う人に任せないとダメだけど、
		デバイスリソースくらいはライブラリ側で面倒見たい…。
		ってなると、ヒープにバックアップ取っておく必要がある。
		テクスチャなら、ビデオメモリとヒープ両方に同じものを置いておく。
		…いっそこの方が良い気がする。メモリはすごい使うと思うけど、
		携帯機がメモリ少な目なことくらい開発者なら知ってるはずだし…。

	[14/11/23] エラー処理について
		他のエンジンとかは…
		・Unity … 動的コンパイルできないみたい。
		・Irrihit … ログ書き出した後 return false;
		・GamePlay … ログ書き出した後 throw
		・XNA … CompledEffect 構造体を返す。これがエラーを持っている。
		・Cg … Context (Manager) が、最後に発生したエラーを持っている。
		・SlimDX … throw CompilationException
		・Esenthel Engine
			ShaderCompileTry … return false;
			ShaderCompile … throw

		エラーは文字列として返したいし、警告の場合も考えないとダメ。
		ログファイルへの出力はあり得ないし、Cg みたいにスレッドセーフにできないのもNG。
		とすると SlimDX を参考にするのが素直か…。
*/

#include "../../../Internal.h"
#include "DX9Module.h"
#include "DX9GraphicsDevice.h"
#include "DX9Shader.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// D3DXInclude
//==============================================================================
class D3DXInclude
	: public ID3DXInclude
{
public:

	D3DXInclude(FileManager* fileManager)
	{
		m_fileManager = fileManager;
	}

	STDMETHOD(Open)(THIS_
		D3DXINCLUDE_TYPE IncludeType,
		LPCSTR pFileName,
		LPCVOID pParentData,
		LPCVOID * ppData,
		UINT * pBytes)
	{
		RefPtr<Stream> file(m_fileManager->CreateFileStream(pFileName), false);

		size_t size = (size_t)file->GetLength();
		byte_t* data = LN_NEW byte_t[size];
		file->Read(data, size);

		*ppData = data;
		*pBytes = size;
		return S_OK;
	}

	STDMETHOD(Close)(THIS_ LPCVOID pData)
	{
		byte_t* data = (byte_t*)pData;
		LN_SAFE_DELETE_ARRAY(data);
		return S_OK;
	}

private:
	FileManager*	m_fileManager;
};


//==============================================================================
// DX9Shader
//==============================================================================

const char* DX9Shader::Macro_LN_HLSL_DX9 = "LN_HLSL_DX9";

//------------------------------------------------------------------------------
ShaderCompileResultLevel DX9Shader::Create(DX9GraphicsDevice* device, const char* code, size_t codeByteCount, DX9Shader** outShader, StringA* outMessage)
{
	// UTF-8のBOM対策
	if (code[0] == 0xEF && code[1] == 0xBB && code[2] == 0xBF) {
		code += 3;
		codeByteCount -= 3;
	}

	// 定義マクロ
	D3DXMACRO deflist[] =
	{
		{ Macro_LN_HLSL_DX9, "1" },
		{ NULL, NULL },
	};

	// シェーダプログラムのコンパイル
	D3DXInclude dxInclude(device->GetFileManager());
	ID3DXBuffer* buffer = NULL;
	ID3DXEffect* dxEffect;
	HRESULT hr = DX9Module::D3DXCreateEffect(
		device->GetIDirect3DDevice9(),
		code,
		codeByteCount,
		deflist,
		&dxInclude,
		D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,	// これがないと古いシェーダをコンパイルしたときにエラーが出る可能性がある (X3025 とか)
		device->GetD3DXEffectPool(),
		&dxEffect,
		&buffer);

	// 失敗
	if (FAILED(hr))
	{
		if (buffer != NULL)
		{
			*outMessage = static_cast<const char*>(buffer->GetBufferPointer());
			LN_SAFE_RELEASE(buffer);
		}
		return ShaderCompileResultLevel_Error;
	}
	// 警告
	else if (buffer != NULL)
	{
		*outMessage = static_cast<const char*>(buffer->GetBufferPointer());
		LN_SAFE_RELEASE(buffer);
		*outShader = LN_NEW DX9Shader(device, dxEffect);
		return ShaderCompileResultLevel_Warning;
	}

	// 成功
	*outShader = LN_NEW DX9Shader(device, dxEffect);
	return ShaderCompileResultLevel_Success;
}

//------------------------------------------------------------------------------
DX9Shader::DX9Shader(DX9GraphicsDevice* device, ID3DXEffect* dxEffect)
	: m_device(device)
	, m_dxEffect(dxEffect)
{
	// すべての変数を列挙
	D3DXHANDLE handle;
	UINT idx = 0;
	while (true)
	{
		handle = m_dxEffect->GetParameter(NULL, idx);
		if (!handle) break;

		auto* v = LN_NEW DX9ShaderVariable(this, handle);
		if (v->GetType() == ShaderVariableType_Unknown)
		{
			LN_SAFE_RELEASE(v);
		}
		else
		{
			m_variables.Add(v);
			if (v->GetType() == ShaderVariableType_DeviceTexture)
			{
				TextureVarInfo info = { v, device->GetDummyTextures()[m_textureVariables.GetCount()], nullptr };
				m_textureVariables.Add(info);
			}
		}

		++idx;
	}

	// すべてのテクニックを列挙
	D3DXHANDLE tech = NULL;
	D3DXHANDLE next = NULL;
	do
	{
		m_dxEffect->FindNextValidTechnique(tech, &next);
		if (next) {
			m_techniques.Add(LN_NEW DX9ShaderTechnique(this, next));
		}
		tech = next;
	} while (tech);
}

//------------------------------------------------------------------------------
DX9Shader::~DX9Shader()
{
	LN_FOREACH(DX9ShaderVariable* v, m_variables) {
		v->Release();
	}

	LN_FOREACH(DX9ShaderTechnique* t, m_techniques) {
		t->Release();
	}
	LN_SAFE_RELEASE(m_dxEffect);
}

//------------------------------------------------------------------------------
IShaderVariable* DX9Shader::GetVariable(int index) const
{
	return m_variables[index]; 
}

//------------------------------------------------------------------------------
IShaderTechnique* DX9Shader::GetTechnique(int index) const
{
	return m_techniques[index];
}

//------------------------------------------------------------------------------
void DX9Shader::OnLostDevice()
{
	LN_COMCALL(m_dxEffect->OnLostDevice());
}

//------------------------------------------------------------------------------
void DX9Shader::OnResetDevice()
{
	LN_COMCALL(m_dxEffect->OnResetDevice());
}

//------------------------------------------------------------------------------
//const SamplerState* DX9Shader::FindSamplerState(IDirect3DBaseTexture9* dxTexture) const
//{
//	for (DX9ShaderVariable* v : m_textureVariables)
//	{
//		DX9TextureBase* tex = static_cast<DX9TextureBase*>(v->GetTexture());
//		if (tex != nullptr && tex->GetIDirect3DBaseTexture9() == dxTexture)
//		{
//			return &tex->GetSamplerState();
//		}
//	}
//	return nullptr;
//}

//==============================================================================
// DX9Shader
//==============================================================================

void D3DXParamDescToLNParamDesc(
	const D3DXPARAMETER_DESC&   dx_desc_,
	//LNShaderVariableClass*      ln_class_,
	//LNShaderVariableType*       ln_type_,
	ShaderVariableType*			type)
{
	// 変数型
	switch (dx_desc_.Type)
	{
	case D3DXPT_VOID:			*type = ShaderVariableType_Unknown; break;
	case D3DXPT_BOOL:			*type = ShaderVariableType_Bool; break;
	case D3DXPT_INT:			*type = ShaderVariableType_Int;	break;
	case D3DXPT_FLOAT:			*type = ShaderVariableType_Float; break;
	case D3DXPT_STRING:			*type = ShaderVariableType_String; break;
	case D3DXPT_TEXTURE:		*type = ShaderVariableType_DeviceTexture; break;
	case D3DXPT_TEXTURE1D:		*type = ShaderVariableType_DeviceTexture; break;
	case D3DXPT_TEXTURE2D:		*type = ShaderVariableType_DeviceTexture; break;
	case D3DXPT_TEXTURE3D:		*type = ShaderVariableType_DeviceTexture; break;
	case D3DXPT_TEXTURECUBE:	*type = ShaderVariableType_DeviceTexture; break;
	case D3DXPT_SAMPLER:		*type = ShaderVariableType_Unknown; break;
	case D3DXPT_SAMPLER1D:		*type = ShaderVariableType_Unknown; break;
	case D3DXPT_SAMPLER2D:		*type = ShaderVariableType_Unknown; break;
	case D3DXPT_SAMPLER3D:		*type = ShaderVariableType_Unknown; break;
	case D3DXPT_SAMPLERCUBE:	*type = ShaderVariableType_Unknown; break;
	case D3DXPT_PIXELSHADER:	*type = ShaderVariableType_Unknown; break;
	case D3DXPT_VERTEXSHADER:	*type = ShaderVariableType_Unknown; break;
	case D3DXPT_PIXELFRAGMENT:	*type = ShaderVariableType_Unknown; break;
	case D3DXPT_VERTEXFRAGMENT:	*type = ShaderVariableType_Unknown; break;
	case D3DXPT_UNSUPPORTED:	*type = ShaderVariableType_Unknown; break;
	default:					*type = ShaderVariableType_Unknown; break;
	}

	// 変数クラス
	if (dx_desc_.Type == D3DXPT_FLOAT)
	{
		if (dx_desc_.Class == D3DXPC_VECTOR)
		{
			*type = ShaderVariableType_Vector;
		}
		else if (
			dx_desc_.Class == D3DXPC_MATRIX_ROWS ||
			dx_desc_.Class == D3DXPC_MATRIX_COLUMNS)
		{
			*type = ShaderVariableType_Matrix;
		}
	}
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::GetValue(ID3DXEffect* dxEffect, D3DXHANDLE handle, ShaderVariableTypeDesc desc, ShaderValue* outValue)
{
	switch (desc.Type)
	{
	case ShaderVariableType_Bool:
	{
		BOOL b;
		LN_COMCALL(dxEffect->GetBool(handle, &b));
		outValue->SetBool(b != FALSE);
		break;
	}
	case ShaderVariableType_BoolArray:
	{
		Array<BOOL> tmp1;
		std::vector<bool> tmp2;	// TODO: Array<bool> がつかえないので
		tmp1.Resize(desc.Elements);
		tmp2.resize(desc.Elements);
		LN_COMCALL(dxEffect->GetBoolArray(handle, (BOOL*)&tmp1[0], desc.Elements));
		for (int i = 0; i < desc.Elements; ++i) tmp2[i] = (tmp1[i] != FALSE);
		outValue->SetBoolArray((bool*)&tmp2[0], desc.Elements);	
		break;
	}
	case ShaderVariableType_Int:
	{
		INT v;
		LN_COMCALL(dxEffect->GetInt(handle, &v));
		outValue->SetInt(v);
		break;
	}
	case ShaderVariableType_Float:
	{
		FLOAT v;
		LN_COMCALL(dxEffect->GetFloat(handle, &v));
		outValue->SetFloat(v);
		break;
	}
	case ShaderVariableType_FloatArray:
	{
		outValue->SetFloatArray(NULL, desc.Elements);	// reserve
		LN_COMCALL(dxEffect->GetFloatArray(handle, (FLOAT*)outValue->GetDataBuffer(), desc.Elements));
		break;
	}
	case ShaderVariableType_Vector:
	{
		Vector4 v;
		LN_COMCALL(dxEffect->GetVector(handle, (D3DXVECTOR4*)&v));
		outValue->SetVector(v);
		break;
	}
	case ShaderVariableType_VectorArray:
	{
		outValue->SetVectorArray(NULL, desc.Elements);	// reserve
		LN_COMCALL(dxEffect->GetVectorArray(handle, (D3DXVECTOR4*)outValue->GetDataBuffer(), desc.Elements));
		break;
	}
	case ShaderVariableType_Matrix:
	{
		Matrix v;
		LN_COMCALL(dxEffect->GetMatrix(handle, (D3DXMATRIX*)&v));
		outValue->SetMatrix(v);
		break;
	}
	case ShaderVariableType_MatrixArray:
	{
		outValue->SetMatrixArray(NULL, desc.Elements);	// reserve
		LN_COMCALL(dxEffect->GetMatrixArray(handle, (D3DXMATRIX*)outValue->GetDataBuffer(), desc.Elements));
		break;
	}
	case ShaderVariableType_DeviceTexture:
	{
		// Texture 型の初期値は NULL。変数の型をセットするため、明示的にセットしておく。
		outValue->SetDeviceTexture(NULL);
		break;
	}
	case ShaderVariableType_String:
	{
		LPCSTR str;
		LN_COMCALL(dxEffect->GetString(handle, &str));
		outValue->SetString(str);
		break;
	}
	}
}

//------------------------------------------------------------------------------
DX9ShaderVariable::DX9ShaderVariable(DX9Shader* owner, D3DXHANDLE handle)
	: m_dxEffect(owner->GetID3DXEffect())
	, m_handle(handle)
{
	// 変数名とセマンティクス名
	D3DXPARAMETER_DESC dxDesc;
	m_dxEffect->GetParameterDesc(m_handle, &dxDesc);
	//mName         = LRefTString( desc.Name );
	//mSemanticName = LRefTString( desc.Semantic );

	// 変数情報
	ShaderVariableTypeDesc desc;
	D3DXParamDescToLNParamDesc(
		dxDesc,
		&desc.Type);
	desc.Rows = dxDesc.Rows;
	desc.Columns = dxDesc.Columns;
	desc.Elements = dxDesc.Elements;
	desc.Shared = (dxDesc.Flags & D3DX_PARAMETER_SHARED);

	// 配列要素数があれば Array になる
	if (desc.Type == ShaderVariableType_Bool ||
		desc.Type == ShaderVariableType_Float ||
		desc.Type == ShaderVariableType_Vector ||
		desc.Type == ShaderVariableType_Matrix)
	{
		if (desc.Elements > 0)
		{
			if (desc.Type == ShaderVariableType_Bool)			desc.Type = ShaderVariableType_BoolArray;
			else if (desc.Type == ShaderVariableType_Float)		desc.Type = ShaderVariableType_FloatArray;
			else if (desc.Type == ShaderVariableType_Vector)	desc.Type = ShaderVariableType_VectorArray;
			else if (desc.Type == ShaderVariableType_Matrix)	desc.Type = ShaderVariableType_MatrixArray;
		}
	}

	// 初期化
	ShaderVariableBase::Initialize(desc, String(dxDesc.Name), String(dxDesc.Semantic));

	// 初期値を読み取る
	GetValue(m_dxEffect, m_handle, desc, &m_value);

	// すべてのアノテーションを配列と map に格納
	D3DXHANDLE anno;
	for (UINT i = 0; i < dxDesc.Annotations; ++i)
	{
		anno = m_dxEffect->GetAnnotation(m_handle, i);
		m_annotations.Add(LN_NEW DX9ShaderAnnotation(owner, anno));
	}

	// テクスチャ型の場合はテクスチャとサンプラステートを記憶する構造体を作る
	//if (LN_SVT_TEXTURE_BEGIN <= mVariableTypeDesc.Type && mVariableTypeDesc.Type <= LN_SVT_TEXTURE_END)
	//{
	//	mTextureParam = LN_NEW TextureParam;
	//	mTextureParam->Texture = NULL;
	//}

}

//------------------------------------------------------------------------------
DX9ShaderVariable::~DX9ShaderVariable()
{
	LN_FOREACH(DX9ShaderAnnotation* anno, m_annotations) {
		anno->Release();
	}
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetBool(bool value)
{
	LN_COMCALL(m_dxEffect->SetBool(m_handle, value));
	ShaderVariableBase::SetBool(value);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetBoolArray(const bool* values, int count)
{
	if (count > 0)
	{
		m_temp.Resize(count);
		for (int i = 0; i < count; ++i)
		{
			m_temp[i] = values[i];
		}
		LN_COMCALL(m_dxEffect->SetBoolArray(m_handle, &m_temp[0], count));
	}
	ShaderVariableBase::SetBoolArray(values, count);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetInt(int value)
{
	LN_COMCALL(m_dxEffect->SetInt(m_handle, value));
	ShaderVariableBase::SetInt(value);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetFloat(float value)
{
	LN_COMCALL(m_dxEffect->SetFloat(m_handle, value));
	ShaderVariableBase::SetFloat(value);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetFloatArray(const float* values, int count)
{
	LN_COMCALL(m_dxEffect->SetFloatArray(m_handle, values, count));
	ShaderVariableBase::SetFloatArray(values, count);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetVector(const Vector4& vec)
{
	LN_COMCALL(m_dxEffect->SetVector(m_handle, (D3DXVECTOR4*)&vec));
	ShaderVariableBase::SetVector(vec);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetVectorArray(const Vector4* vectors, int count)
{
	LN_COMCALL(m_dxEffect->SetVectorArray(m_handle, (const D3DXVECTOR4*)vectors, count));
	ShaderVariableBase::SetVectorArray(vectors, count);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetMatrix(const Matrix& matrix)
{
	LN_COMCALL(m_dxEffect->SetMatrix(m_handle, (const D3DXMATRIX*)&matrix));
	ShaderVariableBase::SetMatrix(matrix);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetMatrixArray(const Matrix* matrices, int count)
{
	LN_COMCALL(m_dxEffect->SetMatrixArray(m_handle, (const D3DXMATRIX*)matrices, count));
	ShaderVariableBase::SetMatrixArray(matrices, count);
}

//------------------------------------------------------------------------------
void DX9ShaderVariable::SetTexture(ITexture* texture)
{
	if (texture != NULL) {
		LN_COMCALL(m_dxEffect->SetTexture(m_handle, static_cast<DX9TextureBase*>(texture)->GetIDirect3DBaseTexture9()));
	}
	else {
		m_dxEffect->SetTexture(m_handle, NULL);
	}
	ShaderVariableBase::SetTexture(texture);

	m_texture = static_cast<DX9TextureBase*>(texture);
}

//------------------------------------------------------------------------------
IShaderVariable* DX9ShaderVariable::GetAnnotation(int index)
{
	return m_annotations[index];
}


//==============================================================================
// DX9ShaderAnnotation
//==============================================================================

//------------------------------------------------------------------------------
DX9ShaderAnnotation::DX9ShaderAnnotation(DX9Shader* owner, D3DXHANDLE handle)
	: DX9ShaderVariable(owner, handle)
{
}

//------------------------------------------------------------------------------
DX9ShaderAnnotation::~DX9ShaderAnnotation()
{
}


//==============================================================================
// DX9ShaderTechnique
//==============================================================================

//------------------------------------------------------------------------------
DX9ShaderTechnique::DX9ShaderTechnique(DX9Shader* owner, D3DXHANDLE handle)
	: m_dxEffect(owner->GetID3DXEffect())
	, m_handle(handle)
{
	D3DXTECHNIQUE_DESC desc;
	m_dxEffect->GetTechniqueDesc(m_handle, &desc);
	m_name = desc.Name;

	// アノテーション
	D3DXHANDLE anno;
	for (UINT i = 0; i < desc.Annotations; ++i)
	{
		anno = m_dxEffect->GetAnnotation(m_handle, i);
		m_annotations.Add(LN_NEW DX9ShaderAnnotation(owner, anno));
	}

	// パス
	D3DXHANDLE pass;
	for (UINT i = 0; i < desc.Passes; ++i)
	{
		pass = m_dxEffect->GetPass(m_handle, i);
		m_passes.Add(LN_NEW DX9ShaderPass(owner, pass, i, m_handle));
	}
}

//------------------------------------------------------------------------------
DX9ShaderTechnique::~DX9ShaderTechnique()
{
	LN_FOREACH(DX9ShaderAnnotation* anno, m_annotations) {
		anno->Release();
	}
	LN_FOREACH(DX9ShaderPass* pass, m_passes) {
		pass->Release();
	}
}

//------------------------------------------------------------------------------
IShaderPass* DX9ShaderTechnique::GetPass(int index)
{
	return m_passes[index];
}


//==============================================================================
// DX9ShaderPass
//==============================================================================

//------------------------------------------------------------------------------
DX9ShaderPass::DX9ShaderPass(DX9Shader* owner, D3DXHANDLE handle, int passIndex, D3DXHANDLE tech)
	: m_owner(owner)
	, m_renderer(static_cast<DX9Renderer*>(owner->GetGraphicsDevice()->GetRenderer()))
	, m_dxEffect(owner->GetID3DXEffect())
	, m_handle(handle)
	, m_technique(tech)
	, m_passIndex(passIndex)
	, m_name()
	, m_annotations()
	, m_resolvedSamplerLink(false)
{
	D3DXPASS_DESC desc;
	m_dxEffect->GetPassDesc(m_handle, &desc);
	m_name = String(desc.Name);

	// すべてのアノテーションを配列と map に格納
	D3DXHANDLE anno;
	for (UINT i = 0; i < desc.Annotations; ++i)
	{
		anno = m_dxEffect->GetAnnotation(m_handle, i);
		m_annotations.Add(LN_NEW DX9ShaderAnnotation(owner, anno));
	}

	// サンプラ変数が指しているサンプラインデックスを取得しておく (もしかしたら連番でもいいのかもしれないが、そんなこと明記されていないので一応)
	ID3DXConstantTable* constantTablePS;
	DX9Module::D3DXGetShaderConstantTable(desc.pPixelShaderFunction, &constantTablePS);
	D3DXCONSTANTTABLE_DESC constDesc;
	constantTablePS->GetDesc(&constDesc);
	for (UINT i = 0; i < constDesc.Constants; ++i)
	{
		D3DXHANDLE handle = constantTablePS->GetConstant(NULL, i);
		D3DXCONSTANT_DESC cd;
		constantTablePS->GetConstantDesc(handle, &cd, NULL);
		if (cd.RegisterSet == D3DXRS_SAMPLER)
		{
			SamplerLink link = { (int)constantTablePS->GetSamplerIndex(handle), nullptr };
			m_samplerLinkList.Add(link);
		}
		//printf("%s\n", cd.Name);	// これでサンプラ変数が取れる
	}
	constantTablePS->Release();

#if 0
	printf("----------------\n");
	LPD3DXCONSTANTTABLE constantTable;
	D3DXGetShaderConstantTable(desc.pVertexShaderFunction, &constantTable);
	

	D3DXCONSTANTTABLE_DESC constDesc;
	constantTable->GetDesc(&constDesc);
	//_p(constDesc.Creator);
	//_p(constDesc.Version);
	//_p(constDesc.Constants);
#pragma comment(lib, "d3dx9.lib")
	for (int i = 0; i < constDesc.Constants; ++i)
	{
		printf("--------\n");
		D3DXHANDLE handle = constantTable->GetConstant(NULL, i);
		D3DXCONSTANT_DESC cd;
		constantTable->GetConstantDesc(handle, &cd, NULL);
	}
#endif
}

//------------------------------------------------------------------------------
DX9ShaderPass::~DX9ShaderPass()
{
	LN_FOREACH(DX9ShaderAnnotation* anno, m_annotations) {
		anno->Release();
	}
}

//------------------------------------------------------------------------------
void DX9ShaderPass::Apply()
{
//	m_renderer->TryBeginScene();
	CommitSamplerStatus();

	auto* current = m_renderer->GetCurrentShaderPass();
	if (current == this) {
		m_dxEffect->CommitChanges();
	}
	else 
	{
		if (current != nullptr) {
			current->EndPass();
		}
		UINT dummy;
		LN_COMCALL(m_dxEffect->SetTechnique(m_technique));
		LN_COMCALL(m_dxEffect->Begin(&dummy, 0));
		LN_COMCALL(m_dxEffect->BeginPass(m_passIndex));
		m_renderer->SetCurrentShaderPass(this);		// 前の Pass の EndPass() が呼ばれる
	}
	
}

//------------------------------------------------------------------------------
void DX9ShaderPass::EndPass()
{
	m_dxEffect->EndPass();
	m_dxEffect->End();
}

//------------------------------------------------------------------------------
void DX9ShaderPass::CommitSamplerStatus()
{
	IDirect3DDevice9* dxDevice = m_owner->GetGraphicsDevice()->GetIDirect3DDevice9();

	// 最初の1回だけ、このパス内で、サンプラインデックスに対応する DX9ShaderVariable を探す。
	// 描画スレッドを使っている場合は初期化時に行うことはできないため、ここで行っている。
	if (!m_resolvedSamplerLink)
	{
		auto* infoList = m_owner->GetTextureVarInfoList();

		// 元のテクスチャを覚えておき、検索用のダミーをセット
		for (auto& info : *infoList)
		{
			LN_COMCALL(m_dxEffect->GetTexture(info.variable->GetHandle(), &info.originalTexture));
			LN_COMCALL(m_dxEffect->SetTexture(info.variable->GetHandle(), info.key));
		}

		LN_COMCALL(m_dxEffect->SetTechnique(m_technique));
		UINT dummy;
		LN_COMCALL(m_dxEffect->Begin(&dummy, D3DXFX_DONOTSAVESTATE | D3DXFX_DONOTSAVESHADERSTATE));
		LN_COMCALL(m_dxEffect->BeginPass(m_passIndex));

		for (int i = 0; i < m_samplerLinkList.GetCount(); ++i)
		{
			SamplerLink* info = &m_samplerLinkList[i];
			IDirect3DBaseTexture9* key;
			dxDevice->GetTexture(info->samplerIndex, &key);
			for (int iVarInfo = 0; iVarInfo < infoList->GetCount(); ++iVarInfo)
			{
				if (infoList->GetAt(iVarInfo).key == key)
				{
					info->variable = infoList->GetAt(iVarInfo).variable;
					break;
				}
			}
		}

		m_dxEffect->EndPass();
		m_dxEffect->End();

		// 元のテクスチャに戻す
		for (auto& info : *infoList)
		{
			m_dxEffect->SetTexture(info.variable->GetHandle(), info.originalTexture);
			LN_SAFE_RELEASE(info.originalTexture);
		}

		m_resolvedSamplerLink = true;
	}

	// サンプラステートの設定
	for (SamplerLink& info : m_samplerLinkList)
	{
		if (info.variable != nullptr)
		{
			DX9TextureBase* tex = info.variable->GetDX9TextureBase();
			if (tex != nullptr)
			{
				const SamplerState& ss = tex->GetSamplerState();
				int index = info.samplerIndex;

				// フィルタモード
				{
					D3DTEXTUREFILTERTYPE table[] =
					{
						D3DTEXF_POINT,	// TextureFilterMode_Point
						D3DTEXF_LINEAR,	// TextureFilterMode_Linear
					};
					dxDevice->SetSamplerState(index, D3DSAMP_MAGFILTER, table[ss.FilterMode]);
					dxDevice->SetSamplerState(index, D3DSAMP_MINFILTER, table[ss.FilterMode]);
					dxDevice->SetSamplerState(index, D3DSAMP_MIPFILTER, table[ss.FilterMode]);
				}
				// ラップモード
				{
					D3DTEXTUREADDRESS table[] =
					{
						D3DTADDRESS_WRAP,	// TextureWrapMode_Repeat
						D3DTADDRESS_CLAMP,	// TextureWrapMode_Clamp
					};
					dxDevice->SetSamplerState(index, D3DSAMP_ADDRESSU, table[ss.FilterMode]);
					dxDevice->SetSamplerState(index, D3DSAMP_ADDRESSV, table[ss.FilterMode]);
					dxDevice->SetSamplerState(index, D3DSAMP_ADDRESSW, table[ss.FilterMode]);
				}
			}
		}
	}
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

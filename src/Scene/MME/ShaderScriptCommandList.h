
#pragma once

#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include <Lumino/Graphics/Shader.h>
#include "MMETypes.h"

namespace Lumino
{
namespace Scene
{
class MMEShader;

//class IMMESubsetRenerer
//{
//public:
//	virtual void OnDrawSubset(int subsetIndex, void* userData) = 0;
//};

/**
	@note	Script が明示的に指定されていない場合は、
			・Technique	の場合 … 全ての子 Pass のコマンドリストを実行する
			・Pass の場合 … "Draw=Geometry" を実行する

			Technique か Pass かは、子コマンドリストが存在するかで判別される。
*/
class ShaderScriptCommandList
	: public RefObject
{
public:

	struct DrawParams
	{
		//Graphics::Renderer*			Renderer;
		//Graphics::GeometryRenderer*	GeometryRenderer;
		RenderingParams*			Params;
		//int							SubsetCount;
		SceneNode*					RenderingNode;
		int							SubsetIndex;
		//IMMESubsetRenerer*			SubsetRenderer;
		//void*						UserData;
		//const Array<int>*			SubsetNumbers;	///< 親から渡された描画するべきサブセット番号リスト (内部用。外部からは NULL を渡すこと)
	};

	enum ValidationError
	{
		ValidationError_Success = 0,
		ValidationError_InvalidRenderTargetIndex,			///< レンダリングターゲットのインデックスが不正
		ValidationError_ScriptExternal_Color_InvalidCall,	///< "ScriptExternal=Color" が postscript ではないシェーダから呼び出された
		ValidationError_ScriptExternal_Color_MultipleCall,	///< "ScriptExternal=Color" が 2回以上呼び出された。
		ValidationError_ScriptExternal_Color_CallInLoop,	///< "ScriptExternal=Color" が Loop 内で呼び出された。
		ValidationError_Pass_CallByPass,					///< "Pass" がパス上で呼び出された。
		ValidationError_Loop_CallByPass,					///< "LoopByCount" "LoopEnd" "LoopGetIndex" が パス上で呼び出された。
		ValidationError_LoopBlockUnMath,					///< "LoopByCount" ～ "LoopEnd" のペアが合わない
		ValidationError_DrawGeometry_CallByTechnique,		///< "DrawGeometry" が テクニック上で呼び出された。
		ValidationError_DrawGeometry_InvalidCall,			///< "DrawGeometry" が "scene" シェーダから呼び出された。 
		ValidationError_DrawBuffer_CallByTechnique,			///< "DrawBuffer" が テクニック上で呼び出された。
		ValidationError_DrawBuffer_InvalidCall,				///< "DrawBuffer" が "object" シェーダから呼び出された。 
	};

public:
	ShaderScriptCommandList();
	~ShaderScriptCommandList();

public:

	/// 初期化
	void Initialize(MMEShader* ownerShader);

	/// テクニックの子パスのコマンドストを追加する
	void AddChildPassCommandList(ShaderScriptCommandList* commandList);

	/// 描画するサブセット番号の配列を設定する (テクニック用)
	//void SetDrawingSubsetNumbers(const Array<int>& numbers);

	/// このコマンドリストを実行する
	void Execute(DrawParams& params);

	/// ScriptExternal=Color まで実行する
	void ExecutePreScriptExternal_Color(DrawParams& params);

	/// ScriptExternal=Color から実行する
	void ExecutePostScriptExternal_Color(DrawParams& params);

	/// スクリプトが正しいかを確認する
	ValidationError CheckValid(MMEScriptClass scriptClass, MMEScriptOrder scriptOrder);


public:

	/// レンダリングターゲットを設定する
	/// (textureVariable を NULL にすると、デフォルト(コマンド開始時)にリセット)
	void Add_RenderColorTarget(int index, Graphics::ShaderVariable* textureVariable);

	/// レンダリングターゲットを設定する
	/// (textureVariable を NULL にすると、デフォルト(コマンド開始時)にリセット)
	void Add_RenderDepthStencilTarget(Graphics::ShaderVariable* textureVariable);

	/// レンダリングターゲットをクリアする色を設定する
	void Add_ClearSetColor(const Vector4& color);		// TODO: このあたり、Graphics::IShaderVariable から getVector するように変更

	/// 深度バッファをクリアするZ値を設定する
	void Add_ClearSetDepth(float depth);

	/// レンダリングターゲットをクリアする
	void Add_ClearColor();

	/// 深度ステンシルサーフェイスをクリアする
	void Add_ClearDepth();

	/// ScriptExternal=Color
	void Add_ScriptExternal_Color();

	/// 指定したパスを実行する 
	/// (他の描画コマンドを実行する。commandList を NULL にすると、Draw=Geometry のみを実行する)
	void Add_Pass(Graphics::ShaderPass* pass, ShaderScriptCommandList* commandList);

	/// 指定した回数だけ、スクリプトの一部をループする
	void Add_LoopByCount(int count);

	/// ループ終端
	void Add_LoopEnd();

	/// ループ中のループカウンタの値を、指定したパラメータに設定する
	void Add_LoopGetIndex(Graphics::ShaderVariable* variable);

	/// オブジェクトを描画する
	void Add_DrawGeometry(Graphics::ShaderPass* pass);

	/// レンダリングターゲットのスクリーンと一致するサイズの、長方形のポリゴンを描画する
	void Add_DrawBuffer(Graphics::ShaderPass* pass);

private:

	/// このコマンドリストを実行する (戻り値は最後に実行したコマンドを指すプログラムカウンタ)
	int InternalExecute(DrawParams& param, int programCounter, int currentLoopCount);

	/// Draw=Geometry
	void DrawGeometry(DrawParams& param, Graphics::ShaderPass* pass);

	/// 現在のレンダリングターゲット等の状態を記憶
	void PushCurrentState(DrawParams& param);

	/// 記憶した状態を復帰
	void PopCurrentState(DrawParams& param);

private:

	/// 描画コマンドの種類
	enum CommandType
	{
		COMMAND_Unknown = 0,
		COMMAND_RenderColorTarget,
		COMMAND_RenderDepthStencilTarget,
		COMMAND_ClearSetColor,
		COMMAND_ClearSetDepth,
		COMMAND_ClearColor,
		COMMAND_ClearDepth,
		COMMAND_ScriptExternal_Color,
		COMMAND_Pass,
		COMMAND_LoopByCount,
		COMMAND_LoopEnd,
		COMMAND_LoopGetIndex,
		COMMAND_DrawGeometry,
		COMMAND_DrawBuffer,
	};

	/// 描画コマンド
	struct Command
	{
		CommandType		Type;
		union
		{
			struct
			{
				int							Index;
				Graphics::ShaderVariable*	TextureVariable;	///< レンダリングターゲットを保持しているシェーダ変数
			} RenderColorTarget;

			struct
			{
				Graphics::ShaderVariable*	TextureVariable;	///< 深度バッファを保持しているシェーダ変数
			} RenderDepthStencilTarget;

			struct
			{
				float						Color[4];
			} ClearSetColor;

			struct
			{
				float						Depth;
			} ClearSetDepth;

			struct
			{
				Graphics::ShaderPass*		Pass;
				ShaderScriptCommandList*		CommandList;
			} Pass;

			struct
			{
				int							Count;
			} LoopByCount;

			struct
			{
				Graphics::ShaderVariable*	Variable;			///< ループ回数を格納するシェーダ変数
			} LoopGetIndex;

			struct
			{
				Graphics::ShaderPass*		Pass;
			} DrawGeometry;

			struct
			{
				Graphics::ShaderPass*		Pass;
			} DrawBuffer;
		};
	};

	typedef Array<Command>					CommandArray;
	typedef Array<ShaderScriptCommandList*>	CommandListArray;

private:
	MMEShader*					m_ownerShader;
	CommandArray				m_commandArray;
	//Array<int>				m_drawingSubsetNumbers;
	CommandListArray			m_childCommandListArray;
	ShaderScriptCommandList*		m_parentList;					///< Pass の場合、親テクニックのコマンドリストを指す
	int							m_scriptExternalColorPosition;	///< ScriptExternal=Color のある要素番号
	Graphics::ColorF			m_clearColor;					///< "ClearSetColor=" で設定された値
	float						m_clearDepth;					///< "ClearSetDepth=" で設定された値

	Graphics::Texture*			m_oldRenderTarget[Graphics::Renderer::MaxMultiRenderTargets];
	Graphics::Texture*			m_oldDepthBuffer;
	Graphics::ColorF			m_oldClearColor;
	float						m_oldClearDepth;

};

} // namespace Scene
} // namespace Lumino

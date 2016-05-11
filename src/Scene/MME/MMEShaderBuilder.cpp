
#pragma once
#include "../../Internal.h"
#include <Lumino/Graphics/Shader.h>
#include "MMETypes.h"
#include "MMEShader.h"
#include "MMEShaderTechnique.h"
#include "MMEShaderBuilder.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// MMEShaderBuilder
//=============================================================================

struct LNVerNameDesc
{
    const TCHAR*	SemanticsName;
	const TCHAR*	AlternateName;   ///< シェーダ固定時に使う変数名
};

static LNVerNameDesc g_MMESemanticTable[ MME_MAX_SEMANTICS ] =
{
    { _T( "NONE" ), NULL },  // ダミー
    { _T( "WORLD" ),                                _T( "g_Matrix_w" ) },
    { _T( "VIEW" ),                                 _T( "g_Matrix_v" ) },
    { _T( "PROJECTION" ),                           _T( "g_Matrix_p" ) },
    { _T( "WORLDVIEW" ),                            _T( "g_Matrix_wv" ) },
    { _T( "VIEWPROJECTION" ),                       _T( "g_Matrix_vp" ) },
    { _T( "WORLDVIEWPROJECTION" ),                  _T( "g_Matrix_wvp" ) },      // [ 6 ]

    { _T( "WORLDINVERSE" ),                         _T( "g_Matrix_wi" ) },
    { _T( "VIEWINVERSE" ),                          _T( "g_Matrix_vi" ) },
    { _T( "PROJECTIONINVERSE" ),                    _T( "g_Matrix_pi" ) },
    { _T( "WORLDVIEWINVERSE" ),                     _T( "g_Matrix_wvi" ) },
    { _T( "VIEWPROJECTIONINVERSE" ),                _T( "g_Matrix_vpi" ) },
    { _T( "WORLDVIEWPROJECTIONINVERSE" ),           _T( "g_Matrix_wvpi" ) },     // [ 12 ]

    { _T( "WORLDTRANSPOSE" ),                       _T( "g_Matrix_wt" ) },
    { _T( "VIEWTRANSPOSE" ),                        _T( "g_Matrix_vt" ) },
    { _T( "PROJECTIONTRANSPOSE" ),                  _T( "g_Matrix_pt" ) },
    { _T( "WORLDVIEWTRANSPOSE" ),                   _T( "g_Matrix_wvt" ) },
    { _T( "VIEWPROJECTIONTRANSPOSE" ),              _T( "g_Matrix_vpt" ) },
    { _T( "WORLDVIEWPROJECTIONTRANSPOSE" ),         _T( "g_Matrix_wvpt" ) },     // [ 18 ]

    { _T( "WORLDINVERSETRANSPOSE" ),                _T( "g_Matrix_wit" ) },
    { _T( "VIEWINVERSETRANSPOSE" ),                 _T( "g_Matrix_vit" ) },
    { _T( "PROJECTIONINVERSETRANSPOSE" ),           _T( "g_Matrix_pit" ) },
    { _T( "WORLDVIEWINVERSETRANSPOSE" ),            _T( "g_Matrix_wvit" ) },
    { _T( "VIEWPROJECTIONINVERSETRANSPOSE" ),       _T( "g_Matrix_vpit" ) },
    { _T( "WORLDVIEWPROJECTIONINVERSETRANSPOSE" ),  _T( "g_Matrix_wvpit" ) },    // [ 24 ]

    { _T( "DIFFUSE" ),                              _T( "g_Diffuse" ) },
    { _T( "AMBIENT" ),                              _T( "g_Ambient" ) },
    { _T( "EMISSIVE" ),                             _T( "g_Emissive" ) },
    { _T( "SPECULAR" ),                             _T( "g_Specular" ) },
    { _T( "SPECULARPOWER" ),                        _T( "g_SpecularPower" ) },
    { _T( "TOONCOLOR" ),                            _T( "g_ToonColor" ) },
    { _T( "EDGECOLOR" ),                            _T( "g_EdgeColor" ) },
    { _T( "POSITION" ),                             _T( "g_LightPosition" ) },
    { _T( "DIRECTION" ),                            _T( "g_LightDirection" ) },
    { _T( "MATERIALTEXTURE" ),                      _T( "g_MaterialTexture" ) },
    { _T( "MATERIALSPHEREMAP" ),        NULL },
    { _T( "VIEWPORTPIXELSIZE" ),        NULL },
    { _T( "TIME" ),                     NULL },
    { _T( "ELAPSEDTIME" ),              NULL },
    { _T( "MOUSEPOSITION" ),            NULL },
    { _T( "LEFTMOUSEDOWN" ),            NULL },
    { _T( "MIDDLEMOUSEDOWN" ),          NULL },
    { _T( "RIGHTMOUSEDOWN" ),           NULL },
    { _T( "CONTROLOBJECT" ),            NULL },
    { _T( "RENDERCOLORTARGET" ),        NULL },
    { _T( "RENDERDEPTHSTENCILTARGET" ), NULL },
    { _T( "ANIMATEDTEXTURE" ),          NULL },
    { _T( "OFFSCREENRENDERTARGET" ),    NULL },
    { _T( "TEXTUREVALUE" ),             NULL },
    { _T( "STANDARDSGLOBAL" ),          NULL },

	// MMM
	//{ _T("LIGHTWVPMATRICES"),	NULL },
	//{ _T("LIGHTPOSITIONS"),		NULL },
	//{ _T("LIGHTDIRECTIONS"),	NULL },
	//{ _T("LIGHTENABLES"),		NULL },
	//{ _T("LIGHTZFARS"),			NULL },

    // 以下は独自仕様
    //{ _T( "ORTHOGRAPHIC" ),    _T( "gMatrix_wt" ) },
    //{ _T( "PROJECTION2D" ),     _T( "gMatrix_p2D" ) },
    { _T( "OPACITY" ),          _T( "g_Opacity" ) },
    { _T( "COLORSCALE" ),       _T( "g_ColorScale" ) },
    { _T( "BLENDCOLOR" ),       _T( "g_BlendColor" ) },
    { _T( "TONE" ),             _T( "g_Tone" ) },
    { _T( "UVTRANSFORM" ),      _T( "g_UVTransform" ) }
};

static const char* gMMEAnnotationNames[ MME_MAX_ANNOTATIONS ] =
{
    "NONE",
    "OBJECT",
    "SYNCINEDITMODE",

    "NAME",
    "ITEM",

    "RESOURCETYPE",
    "RESOURCENAME",
    "WIDTH",
    "HEIGHT",
    "DEPTH",
    "DIMENSIONS",
    "VIEWPORTRATIO",
    "FORMAT",
    "MIPLEVELS",
    "LEVELS",

    "OFFSET",
    "SPEED",
    "SEEKVARIABLE",

    "CLEARCOLOR",
    "CLEARDEPTH",
    "ANTIALIAS",
    "DESCRIPTION",
    "DEFAULTEFFECT",

    "TEXTURENAME",

    "SCRIPTOUUTPUT",
    "SCRIPTCLASS",
    "SCRIPTORDER",

    "SCRIPT"
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShader* MMEShaderBuilder::Create(SceneGraphManager* manager, MMEShader* shader, MMEShaderErrorInfo* errorInfo)
{
	MMEShaderBuilder builder(manager, shader, errorInfo);
	builder.Build();
	return builder.m_mmeShader;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderBuilder::MMEShaderBuilder(SceneGraphManager* manager, MMEShader* shader, MMEShaderErrorInfo* errorInfo)
	: m_mmeShader(NULL)
	, m_errorInfo(NULL)
{
	m_mmeShader = shader;
}

MMEShaderBuilder::~MMEShaderBuilder()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderBuilder::Build()
{
	//-----------------------------------------------------
	// シェーダプログラム内のすべての変数をチェックする
	LN_FOREACH(ShaderVariable* var, m_mmeShader->GetVariables())
	{
		// シェーダ変数。とりあえずでフォルト値を入れておく
		MMEShaderVariable* sv = LN_NEW MMEShaderVariable();
		sv->Variable = var;
		sv->Request = MME_VARREQ_NONE;
		//sv->ControlObject       = NULL;
		sv->TextureObject = NULL;
		sv->LightNum = 0;
		sv->LightParamIsMatrix = false;
		m_mmeShader->m_mmeShaderVariableList.Add(sv);

		// セマンティクス確認。CONTROLOBJECT の特殊な値だった場合は sv の ObjectName 等に値が入る
		MMEScriptOutput op1;
		MMEScriptClass  op2;
		MMEScriptOrder  op3;
		bool is_controllobject;
		CheckVariableRequest(
			var,
			sv,
			&op1,
			&op2,
			&op3,
			&is_controllobject);
		if (sv->Request == MME_VARREQ_STANDARDSGLOBAL)
		{
			m_mmeShader->m_mmeScriptOutput = op1;
			m_mmeShader->m_mmeScriptClass = op2;
			m_mmeShader->m_mmeScriptOrder = op3;
		}
		if (is_controllobject)
		{
			m_mmeShader->m_controlObjectVariables.Add(sv);
		}

		// 負荷軽減のため、必要な行列演算をチェックする
		m_mmeShader->m_worldMatrixCalcMask |= CheckMatrixMask(sv->Request);

		// 必要なライト最大数チェック
		if (sv->LightNum > m_mmeShader->m_requiredLightCount) {
			m_mmeShader->m_requiredLightCount = sv->LightNum;
		}
	}

	//-----------------------------------------------------
	// テクニック情報作成
	LN_FOREACH(ShaderTechnique* tech, m_mmeShader->GetTechniques())
	{
		MMEShaderTechnique* sstech = LN_NEW MMEShaderTechnique();
		sstech->Initialize(m_mmeShader, tech, m_errorInfo);
		m_mmeShader->m_mmeShaderTechniqueList.Add(sstech);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShaderBuilder::CheckVariableRequest(
	ShaderVariable* var,
	MMEShaderVariable* sv,
	MMEScriptOutput* script_output,
	MMEScriptClass* script_class,
	MMEScriptOrder* script_order,
	bool* is_controllobject)
{
	*is_controllobject = false;

	//const TCHAR*		name;                           // シェーダから文字列を受け取る変数
	//TCHAR				temp[MAX_SEMANTICS_NAME_LEN]; // 文字列のコピーに使う一時バッファ
	MMEVariableRequest  req = MME_VARREQ_NONE;          // 戻り値

	//MMESemanticsMap::const_iterator it;
	//MMESemanticsMap::const_iterator end = semantics_map_.end();
	//const LNShaderVariableTypeDesc& var_desc = var->getTypeDesc();  // 変数の型情報


	//name = var->getSemanticName();
	int r;

	switch (GetMMESemanticBySemanticName(var->GetSemanticName()))
	{
		/////////////////////////////////////// 変換行列
	case MME_SEMANTIC_WORLD:
		req = MME_VARREQ_MATRIX_World;
		break;
	case MME_SEMANTIC_VIEW:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_View : MME_VARREQ_MATRIX_LIGHT_View;
		break;
	case MME_SEMANTIC_PROJECTION:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_Proj : MME_VARREQ_MATRIX_LIGHT_Proj;
		break;
	case MME_SEMANTIC_WORLDVIEW:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldView : MME_VARREQ_MATRIX_LIGHT_WorldView;
		break;
	case MME_SEMANTIC_VIEWPROJECTION:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_ViewProj : MME_VARREQ_MATRIX_LIGHT_ViewProj;
		break;
	case MME_SEMANTIC_WORLDVIEWPROJECTION:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj : MME_VARREQ_MATRIX_LIGHT_WorldViewProj;
		break;

		///////////////////////////////////////  逆行列
	case MME_SEMANTIC_WORLD_I:
		req = MME_VARREQ_MATRIX_World_I;
		break;
	case MME_SEMANTIC_VIEW_I:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_View_I : MME_VARREQ_MATRIX_LIGHT_View_I;
		break;
	case MME_SEMANTIC_PROJECTION_I:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_Proj_I : MME_VARREQ_MATRIX_LIGHT_Proj_I;
		break;
	case MME_SEMANTIC_WORLDVIEW_I:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldView_I : MME_VARREQ_MATRIX_LIGHT_WorldView_I;
		break;
	case MME_SEMANTIC_VIEWPROJECTION_I:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_I : MME_VARREQ_MATRIX_LIGHT_ViewProj_I;
		break;
	case MME_SEMANTIC_WORLDVIEWPROJECTION_I:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I;
		break;

		///////////////////////////////////////  転置行列
	case MME_SEMANTIC_WORLD_T:
		req = MME_VARREQ_MATRIX_World_T;
		break;
	case MME_SEMANTIC_VIEW_T:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_View_T : MME_VARREQ_MATRIX_LIGHT_View_T;
		break;
	case MME_SEMANTIC_PROJECTION_T:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_Proj_T : MME_VARREQ_MATRIX_LIGHT_Proj_T;
		break;
	case MME_SEMANTIC_WORLDVIEW_T:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldView_T : MME_VARREQ_MATRIX_LIGHT_WorldView_T;
		break;
	case MME_SEMANTIC_VIEWPROJECTION_T:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_T : MME_VARREQ_MATRIX_LIGHT_ViewProj_T;
		break;
	case MME_SEMANTIC_WORLDVIEWPROJECTION_T:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T;
		break;

		///////////////////////////////////////  逆行列×転置行列
	case MME_SEMANTIC_WORLD_IT:
		req = MME_VARREQ_MATRIX_World_IT;
		break;
	case MME_SEMANTIC_VIEW_IT:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_View_IT : MME_VARREQ_MATRIX_LIGHT_View_IT;
		break;
	case MME_SEMANTIC_PROJECTION_IT:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_Proj_IT : MME_VARREQ_MATRIX_LIGHT_Proj_IT;
		break;
	case MME_SEMANTIC_WORLDVIEW_IT:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldView_IT : MME_VARREQ_MATRIX_LIGHT_WorldView_IT;
		break;
	case MME_SEMANTIC_VIEWPROJECTION_IT:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_ViewProj_IT : MME_VARREQ_MATRIX_LIGHT_ViewProj_IT;
		break;
	case MME_SEMANTIC_WORLDVIEWPROJECTION_IT:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT : MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT;
		break;

		/////////////////////////////////////// 色
	case MME_SEMANTIC_DIFFUSE:
		r = CheckAnnotationGeometryOrLight(var);
		if (r != 0) {
			req = (r == 1) ? MME_VARREQ_OBJECT_DIFFUSE : MME_VARREQ_LIGHT_DIFFUSE;
		}
		break;

	case MME_SEMANTIC_AMBIENT:
		r = CheckAnnotationGeometryOrLight(var);
		if (r != 0) {
			req = (r == 1) ? MME_VARREQ_OBJECT_AMBIENT : MME_VARREQ_LIGHT_AMBIENT;
		}
		break;

	case MME_SEMANTIC_EMISSIVE:
		req = MME_VARREQ_OBJECT_EMISSIVE;
		break;

	case MME_SEMANTIC_SPECULAR:
		r = CheckAnnotationGeometryOrLight(var);
		if (r != 0) {
			req = (r == 1) ? MME_VARREQ_OBJECT_SPECULAR : MME_VARREQ_LIGHT_SPECULAR;
		}
		break;

	case MME_SEMANTIC_SPECULARPOWER:
		req = MME_VARREQ_OBJECT_SPECULARPOWER;
		break;

	case MME_SEMANTIC_TOONCOLOR:
		req = MME_VARREQ_OBJECT_TOONCOLOR;
		break;

	case MME_SEMANTIC_EDGECOLOR:
		req = MME_VARREQ_OBJECT_EDGECOLOR;
		break;

		/////////////////////////////////////// 位置・向き
	case MME_SEMANTIC_POSITION:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_CAMERA_POSITION : MME_VARREQ_LIGHT_POSITION;
		break;

	case MME_SEMANTIC_DIRECTION:
		req = (CheckAnnotationCameraOrLight(var)) ? MME_VARREQ_CAMERA_DIRECTION : MME_VARREQ_LIGHT_DIRECTION;
		break;

		/////////////////////////////////////// マテリアルのテクスチャ
	case MME_SEMANTIC_MATERIALTEXTURE:
		req = MME_VARREQ_OBJECT_MATERIALTEXTURE;
		break;

	case MME_SEMANTIC_MATERIALSPHEREMAP:
		req = MME_VARREQ_OBJECT_MATERIALSPHEREMAP;
		break;

		/////////////////////////////////////// misc
	case MME_SEMANTIC_VIEWPORTPIXELSIZE:
		req = MME_VARREQ_VIEWPORTPIXELSIZE;
		break;

	case MME_SEMANTIC_TIME:
		req = MME_VARREQ_TIME;
		break;

	case MME_SEMANTIC_ELAPSEDTIME:
		req = MME_VARREQ_ELAPSEDTIME;
		break;

		/////////////////////////////////////// マウス
	case MME_SEMANTIC_MOUSEPOSITION:
		req = MME_VARREQ_MOUSEPOSITION;
		break;
	case MME_SEMANTIC_LEFTMOUSEDOWN:
		req = MME_VARREQ_LEFTMOUSEDOWN;
		break;
	case MME_SEMANTIC_MIDDLEMOUSEDOWN:
		req = MME_VARREQ_MIDDLEMOUSEDOWN;
		break;
	case MME_SEMANTIC_RIGHTMOUSEDOWN:
		req = MME_VARREQ_RIGHTMOUSEDOWN;
		break;

		/////////////////////////////////////// CONTROLOBJECT
	case MME_SEMANTIC_CONTROLOBJECT:
	{
		ShaderVariable* name_anno = GetAnnotationByName(var, _T("name"));
		if (!name_anno) { // TODO: error name は必須
			break;
		}
		// 操作対象の名前として覚えておく
		sv->ObjectName = name_anno->GetString();

		// "item" をチェック
		ShaderVariable* anno = GetAnnotationByName(var, _T("item"));
		if (anno)
		{
			const TCHAR* item = anno->GetString();
			if (item)
			{
				if (StringTraits::tcsicmp(item, _T("X")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_X;
				}
				else if (StringTraits::tcsicmp(item, _T("Y")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Y;
				}
				else if (StringTraits::tcsicmp(item, _T("Y")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Z;
				}
				else if (StringTraits::tcsicmp(item, _T("XYZ")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_XYZ;
				}
				else if (StringTraits::tcsicmp(item, _T("Rx")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Rx;
				}
				else if (StringTraits::tcsicmp(item, _T("Ry")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Ry;
				}
				else if (StringTraits::tcsicmp(item, _T("Rz")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Rz;
				}
				else if (StringTraits::tcsicmp(item, _T("Rxyz")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Rxyz;
				}
				else if (StringTraits::tcsicmp(item, _T("Si")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Si;
				}
				else if (StringTraits::tcsicmp(item, _T("Tr")) == 0)
				{
					req = MME_VARREQ_CONTROLOBJECT_Tr;
				}
				else if (var->GetType() == ShaderVariableType_Float)
				{
					// 型が float の場合は表情
					req = MME_VARREQ_CONTROLOBJECT_MorphBlend;
					sv->ItemName = item;	// 操作対象の名前として覚えておく
				}
				else if (var->GetType() == ShaderVariableType_Vector && var->GetColumns() >= 3)
				{
					// float3 4 の場合はボーン位置
					req = MME_VARREQ_CONTROLOBJECT_BoneOffset;
					sv->ItemName = item;
				}
				else if (var->GetType() == ShaderVariableType_Matrix && var->GetRows() == 4 && var->GetColumns() == 4)
				{
					// matrix4x4 の場合はボーン行列
					req = MME_VARREQ_CONTROLOBJECT_BoneMatrix;
					sv->ItemName = item;
				}
			} // if ( item )
		} // if ( anno )

		// ここまででまだ req が決まってない場合
		if (req == MME_VARREQ_NONE)
		{
			// 型が bool の場合
			if (var->GetType() == ShaderVariableType_Bool)
			{
				req = MME_VARREQ_CONTROLOBJECT_Visible;
			}
			// float の場合は拡大率
			else if (var->GetType() == ShaderVariableType_Float)
			{
				req = MME_VARREQ_CONTROLOBJECT_Scale;
			}
			// float3 or 4 の場合は位置
			else if (var->GetType() == ShaderVariableType_Vector && var->GetColumns() >= 3)
			{
				req = MME_VARREQ_CONTROLOBJECT_Position;
			}
			// matrix4x4 の場合はワールド行列
			else if (var->GetType() == ShaderVariableType_Matrix && var->GetRows() == 4 && var->GetColumns() == 4)
			{
				req = MME_VARREQ_CONTROLOBJECT_World;
			}

		}

		// ここまでで要求項目が見つかっていたら、有効な CONTROLOBJECT
		if (req != MME_VARREQ_NONE) {
			*is_controllobject = true;
		}
		else {
			//TODO: 認識できない CONTROLOBJECT
		}

		break;
	}


		/////////////////////////////////////// テクスチャ関連
	case MME_SEMANTIC_RENDERCOLORTARGET:
		req = MME_VARREQ_RENDERCOLORTARGET;
		break;
	case MME_SEMANTIC_RENDERDEPTHSTENCILTARGET:
		req = MME_VARREQ_RENDERDEPTHSTENCILTARGET;
		break;
	case MME_SEMANTIC_ANIMATEDTEXTURE:
		printf("ANIMATEDTEXTURE セマンティクスは未対応です。\n");	// TODO:
		req = MME_VARREQ_ANIMATEDTEXTURE;
		break;
	case MME_SEMANTIC_OFFSCREENRENDERTARGET:
		req = MME_VARREQ_OFFSCREENRENDERTARGET;
		break;
	case MME_SEMANTIC_TEXTUREVALUE:
		printf("TEXTUREVALUE セマンティクスは未対応です。\n");	// TODO:
		req = MME_VARREQ_TEXTUREVALUE;
		break;

		/////////////////////////////////////// STANDARDSGLOBAL
	case MME_SEMANTIC_STANDARDSGLOBAL:
	{
		req = MME_VARREQ_STANDARDSGLOBAL;

		const TCHAR* scriptClassName = NULL;
		const TCHAR* scriptOrderName = NULL;
		const TCHAR* script = NULL;

		ShaderVariable* anno = NULL;
		anno = GetAnnotationByName(var, _T("ScriptClass"));
		if (anno) {
			scriptClassName = anno->GetString();
		}
		anno = GetAnnotationByName(var, _T("ScriptOrder"));
		if (anno) {
			scriptOrderName = anno->GetString();
		}
		anno = GetAnnotationByName(var, _T("Script"));
		if (anno) {
			script = anno->GetString();
		}

		*script_output = MME_SCROUT_color;
		*script_class = MME_SCRCLS_object;
		*script_order = MME_SCRORDER_standard;

		// ↑でデフォルト値として代入してるのでコメントアウト
		//if ( stricmp( script_class, "object" ) ) { MME_SCRCLS_object; }
		if (StringTraits::tcsicmp(scriptClassName, _T("scene")) == 0)       {
			*script_class = MME_SCRCLS_scene;
		}
		else if (StringTraits::tcsicmp(scriptClassName, _T("sceneorobject")) == 0) {
			*script_class = MME_SCRCLS_sceneorobject;
		}

		//if ( stricmp( script_order, "object" ) ) { MME_SCRORDER_standard; }
		if (StringTraits::tcsicmp(scriptOrderName, _T("preprocess")) == 0)    {
			*script_order = MME_SCRORDER_preprocess;
		}
		else if (StringTraits::tcsicmp(scriptOrderName, _T("postprocess")) == 0)   {
			*script_order = MME_SCRORDER_postprocess;
		}


		if (script)
		{
			printf("STANDARDSGLOBAL の Script アノテーションは未対応です。\n");	// TODO:
			// Script では使用するテクニックの検索順序を指定する

			LN_THROW(0, NotImplementedException);
		}

		break;
	}

		/////////////////////////////////////// 独自仕様部分
		/*
		case LN_SEMANTIC_ORTHOGRAPHIC:
		{
		req = ( CheckAnnotationCameraOrLight( var ) ) ? LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC : LN_VARREQ_MATRIX_LIGHT_ORTHOGRAPHIC;
		break;
		}
		*/
	//case LN_SEMANTIC_PROJECTION2D:
	//{
	//	req = LN_VARREQ_MATRIX_PROJECTION2D;
	//	break;
	//}
	case LN_SEMANTIC_OPACITY:
	{
		req = LN_VARREQ_OPACITY;
		break;
	}
	case LN_SEMANTIC_COLOR_SCALE:
	{
		req = LN_VARREQ_COLOR_SCALE;
		break;
	}
	case LN_SEMANTIC_BLEND_COLOR:
	{
		req = LN_VARREQ_BLEND_COLOR;
		break;
	}
	case LN_SEMANTIC_TONE:
	{
		req = LN_VARREQ_TONE;
		break;
	}
	case LN_SEMANTIC_UVTRANSFORM:
	{
		req = LN_VARREQ_UVTRANSFORM;
		break;
	}
	} // switch


	// まだ決まっていない場合はセマンティクス無しテクスチャの可能性を調べる
	if (req == MME_VARREQ_NONE)
	{
		//if ( var_desc.Class == LN_SVC_OBJECT && var_desc.Type == LN_SVT_TEXTURE )
		if (var->GetType() == ShaderVariableType_DeviceTexture)
		{
			req = MME_VARREQ_TEXTURE;
		}
	}

	// ライト関係の場合は配列の要素数 = 必要なライトの数を確認する
	sv->LightParamIsMatrix = false;
	switch (req)
	{
	case MME_VARREQ_MATRIX_LIGHT_WorldView:
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
	case MME_VARREQ_MATRIX_LIGHT_View:
	case MME_VARREQ_MATRIX_LIGHT_Proj:
	case MME_VARREQ_MATRIX_LIGHT_ViewProj:
	case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
	case MME_VARREQ_MATRIX_LIGHT_View_I:
	case MME_VARREQ_MATRIX_LIGHT_Proj_I:
	case MME_VARREQ_MATRIX_LIGHT_ViewProj_I:
	case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
	case MME_VARREQ_MATRIX_LIGHT_View_T:
	case MME_VARREQ_MATRIX_LIGHT_Proj_T:
	case MME_VARREQ_MATRIX_LIGHT_ViewProj_T:
	case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
	case MME_VARREQ_MATRIX_LIGHT_View_IT:
	case MME_VARREQ_MATRIX_LIGHT_Proj_IT:
	case MME_VARREQ_MATRIX_LIGHT_ViewProj_IT:
		sv->LightParamIsMatrix = true;
	case MME_VARREQ_LIGHT_DIFFUSE:
	case MME_VARREQ_LIGHT_AMBIENT:
	case MME_VARREQ_LIGHT_SPECULAR:
	case MME_VARREQ_LIGHT_POSITION:
	case MME_VARREQ_LIGHT_DIRECTION:
		// 配列でない
		if (var->GetArrayElements() == 0) {
			sv->LightParamIsArray = false;
			sv->LightNum = 1;
		}
		// 配列である
		else {
			sv->LightParamIsArray = true;
			sv->LightNum = var->GetArrayElements();
		}
		break;
	}

	// 要求項目を返す
	sv->Request = req;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMESemantic MMEShaderBuilder::GetMMESemanticBySemanticName(const String& name)
{
	assert(LN_ARRAY_SIZE_OF(g_MMESemanticTable) == MME_MAX_SEMANTICS);

	if (name.IsEmpty()) {
		return MME_SEMANTIC_NONE;
	}

	// 単純な線形探索。要素数が数百とかでなければ map 使うよりも速い。
	for (int i = 0; i < LN_ARRAY_SIZE_OF(g_MMESemanticTable); ++i)
	{
		// 大文字小文字を区別しない
		if (name.Compare(g_MMESemanticTable[i].SemanticsName, -1, CaseSensitivity::CaseInsensitive) == 0)
		{
			return (MMESemantic)i;
		}
	}
	return MME_SEMANTIC_NONE;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool MMEShaderBuilder::CheckAnnotationCameraOrLight(ShaderVariable* var)
{
	ShaderVariable* anno = GetAnnotationByName(var, _T("Object"));
	if (!anno) {
		return true;	// "Object" アノテーションが見つからなかった
	}

	if (StringTraits::tcsicmp(anno->GetString(), _T("Light")) == 0) {
		return false;
	}
	return true;	// TODO: とあえず、あてはまらなければ "Camera" 扱いにする
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int MMEShaderBuilder::CheckAnnotationGeometryOrLight(ShaderVariable* var)
{
	ShaderVariable* anno = GetAnnotationByName(var, _T("Object"));
	if (!anno) {
		return 0;	// "Object" アノテーションが見つからなかった
	}
	if (StringTraits::tcsicmp(anno->GetString(), _T("Geometry")) == 0) {
		return 1;
	}
	if (StringTraits::tcsicmp(anno->GetString(), _T("Light")) == 0) {
		return 2;
	}
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderVariable* MMEShaderBuilder::GetAnnotationByName(ShaderVariable* var, const TCHAR* name)
{
	LN_FOREACH(ShaderVariable* anno, var->GetAnnotations())
	{
		if (anno->GetName().Compare(name, -1, CaseSensitivity::CaseInsensitive)) {
			return anno;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint32_t MMEShaderBuilder::CheckMatrixMask(MMEVariableRequest req)
{
	uint32_t flags = 0;

	switch (req)
	{
	case MME_VARREQ_MATRIX_CAMERA_WorldView:
		flags = MME_WMCF_WORLD_VIEW_CAMERA; break;
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
		flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldView:
		flags = MME_WMCF_WORLD_VIEW_LIGHT; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
		flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

	case MME_VARREQ_MATRIX_World_I:
		flags = MME_WMCF_WORLD_I; break;
	case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
		flags = MME_WMCF_WORLD_VIEW_CAMERA_I | MME_WMCF_WORLD_VIEW_CAMERA; break;
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
		flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I | MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
		flags = MME_WMCF_WORLD_VIEW_LIGHT_I | MME_WMCF_WORLD_VIEW_LIGHT; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
		flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I | MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

	case MME_VARREQ_MATRIX_World_T:
		flags = MME_WMCF_WORLD_T; break;
	case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
		flags = MME_WMCF_WORLD_VIEW_CAMERA_T | MME_WMCF_WORLD_VIEW_CAMERA; break;
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
		flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T | MME_WMCF_WORLD_VIEW_PROJ_CAMERA; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
		flags = MME_WMCF_WORLD_VIEW_LIGHT_T | MME_WMCF_WORLD_VIEW_LIGHT; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
		flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T | MME_WMCF_WORLD_VIEW_PROJ_LIGHT; break;

	case MME_VARREQ_MATRIX_World_IT:
		flags = MME_WMCF_WORLD_I | MME_WMCF_WORLD_T | MME_WMCF_WORLD_IT; break;
	case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
		flags = MME_WMCF_WORLD_VIEW_CAMERA_I | MME_WMCF_WORLD_VIEW_CAMERA_T | MME_WMCF_WORLD_VIEW_CAMERA_IT; break;
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
		flags = MME_WMCF_WORLD_VIEW_PROJ_CAMERA_I | MME_WMCF_WORLD_VIEW_PROJ_CAMERA_T | MME_WMCF_WORLD_VIEW_PROJ_CAMERA_IT; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
		flags = MME_WMCF_WORLD_VIEW_LIGHT_I | MME_WMCF_WORLD_VIEW_LIGHT_T | MME_WMCF_WORLD_VIEW_LIGHT_IT; break;
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
		flags = MME_WMCF_WORLD_VIEW_PROJ_LIGHT_I | MME_WMCF_WORLD_VIEW_PROJ_LIGHT_T | MME_WMCF_WORLD_VIEW_PROJ_LIGHT_IT; break;
	}
	return flags;
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

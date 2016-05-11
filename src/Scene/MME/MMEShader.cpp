
#include "../../Internal.h"
#include "../SceneGraphManager.h"
#include "MmdMaterial.h"
#include <Lumino/Scene/SceneNode.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/VisualNodeParams.h>
#include "MMEShaderBuilder.h"
#include "MMEShaderTechnique.h"
#include "MMEShader.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// MMEShader
//=============================================================================

static const unsigned char MMM_EffectHeader_Data[] =
{
#include "../Resource/MMM_EffectHeader.fxh.h"
};
static const size_t MMM_EffectHeader_Data_Len = LN_ARRAY_SIZE_OF(MMM_EffectHeader_Data) - 1;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShader* MMEShader::Create(const char* code, int codeLength, MMEShaderErrorInfo* errorInfo, SceneGraphManager* manager)
{
	// TODO: 自動付加するべきだろうか？
	StringA newCode(code, codeLength);
	//StringA newCode((const char*)MMM_EffectHeader_Data, MMM_EffectHeader_Data_Len);
	//newCode += StringA::GetNewLine();
	//newCode += "#line 1";
	//newCode += StringA::GetNewLine();
	//newCode += StringA(code, codeLength);

	RefPtr<MMEShader> mmeShader(LN_NEW MMEShader(manager), false);
	mmeShader->Initialize(manager->GetGraphicsManager(), newCode.c_str(), newCode.GetLength());
	MMEShaderBuilder::Create(manager, mmeShader, errorInfo);
	return mmeShader.DetachMove();

	//RefPtr<Shader> shader(LN_NEW Shader(), false);
	//shader->Initialize(manager->GetGraphicsManager(), newCode.c_str(), newCode.GetLength());
	//RefPtr<MMEShader> mmeShader(MMEShaderBuilder::Create(manager, shader, errorInfo), false);
	//mmeShader.SafeAddRef();
	//return mmeShader;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShader::MMEShader(SceneGraphManager* manager)
	: m_manager(NULL)
	, m_mmeScriptOutput(MME_SCROUT_color)
	, m_mmeScriptClass(MME_SCRCLS_object)
	, m_mmeScriptOrder(MME_SCRORDER_standard)
	, m_mmeShaderVariableList()
	, m_controlObjectVariables()
	, m_worldMatrixCalcMask(0)
	, m_requiredLightCount(0)
	, m_mmeShaderTechniqueList()
{
	LN_REFOBJ_SET(m_manager, manager);
	m_manager->AddShader(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShader::~MMEShader()
{
	LN_FOREACH(MMEShaderTechnique* tech, m_mmeShaderTechniqueList) {
		tech->Release();
	}
	LN_FOREACH(MMEShaderVariable* var, m_mmeShaderVariableList) {
		delete var;
	}

	if (m_manager != NULL) {
		m_manager->RemoveShader(this);
		LN_SAFE_RELEASE(m_manager);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateSceneParams(const MMESceneParams& params, SceneGraphManager* scene)
{
	ShaderVariable* var;
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
		// 変数が配列のライト情報はこの関数内では設定しないので次へ
		if (sv->LightNum >= 1 && sv->LightParamIsArray) {
			continue;
		}

		var = sv->Variable;

		// CONTROLOBJECT
		if (MME_VARREQ_CONTROLOBJECT_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_CONTROLOBJECT_END) {
			if (sv->ObjectName == _T("(self)")) {
				// ノード単位のデータとして別途設定するため、ここでは特に何もしない
			}
			else if (sv->ObjectName == _T("(OffscreenOwner)")) {
				LN_THROW(0, NotImplementedException);
			}
			else {
				IMMESceneObject* obj = scene->FindNodeFirst(sv->ObjectName);
				SetControllObjectParam(sv, obj);
			}
			continue;
		}

		switch (sv->Request)
		{
		case MME_VARREQ_TIME:
			var->SetFloat(params.Time);
			break;
		case MME_VARREQ_ELAPSEDTIME:
			var->SetFloat(params.ElapsedTime);
			break;
		case MME_VARREQ_MOUSEPOSITION:
			var->SetVector(params.MousePosition);
			break;
		case MME_VARREQ_LEFTMOUSEDOWN:
			var->SetVector(params.LeftMouseDown);
			break;
		case MME_VARREQ_MIDDLEMOUSEDOWN:
			var->SetVector(params.MiddleMouseDown);
			break;
		case MME_VARREQ_RIGHTMOUSEDOWN:
			var->SetVector(params.RightMouseDown);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateCameraParams(Camera* camera, const SizeF& viewPixelSize)
{
	ShaderVariable* var;
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
		var = sv->Variable;

		switch (sv->Request)
		{
			// 行列
		case MME_VARREQ_MATRIX_CAMERA_View:
			var->SetMatrix(camera->GetViewMatrix());
			break;
		case MME_VARREQ_MATRIX_CAMERA_Proj:
			var->SetMatrix(camera->GetProjectionMatrix());
			break;
		case MME_VARREQ_MATRIX_CAMERA_ViewProj:
			var->SetMatrix(camera->GetViewProjectionMatrix());
			break;

			// 逆行列
		case MME_VARREQ_MATRIX_CAMERA_View_I:
			var->SetMatrix(camera->GetViewMatrixI());
			break;
		case MME_VARREQ_MATRIX_CAMERA_Proj_I:
			var->SetMatrix(camera->GetProjectionMatrixI());
			break;
		case MME_VARREQ_MATRIX_CAMERA_ViewProj_I:
			var->SetMatrix(camera->GetViewProjectionMatrixI());
			break;

			// 転置行列
		case MME_VARREQ_MATRIX_CAMERA_View_T:
			var->SetMatrix(camera->GetViewMatrixT());
			break;
		case MME_VARREQ_MATRIX_CAMERA_Proj_T:
			var->SetMatrix(camera->GetProjectionMatrixT());
			break;
		case MME_VARREQ_MATRIX_CAMERA_ViewProj_T:
			var->SetMatrix(camera->GetViewProjectionMatrixT());
			break;

			// 逆行列×転置行列
		case MME_VARREQ_MATRIX_CAMERA_View_IT:
			var->SetMatrix(camera->GetViewMatrixIT());
			break;

		case MME_VARREQ_MATRIX_CAMERA_Proj_IT:
			var->SetMatrix(camera->GetProjectionMatrixIT());
			break;

		case MME_VARREQ_MATRIX_CAMERA_ViewProj_IT:
			var->SetMatrix(camera->GetViewProjectionMatrixIT());
			break;
			// カメラの位置・向き
		case MME_VARREQ_CAMERA_POSITION:
			var->SetVector(Vector4(camera->GetPosition(), 0.0f));
			break;
		case MME_VARREQ_CAMERA_DIRECTION:
			var->SetVector(camera->GetDirectionInternal());
			break;

			// 正射影行列
			//case LN_VARREQ_MATRIX_CAMERA_ORTHOGRAPHIC:
			//    var->SetMatrix( params.Mat_CameraOrtho );
			//    break;

			// 描画先RTサイズ
		case MME_VARREQ_VIEWPORTPIXELSIZE:
			var->SetVector(Vector4(viewPixelSize.width, viewPixelSize.height, 0, 0));
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateNodeParams(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList)
{
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
		// CONTROLOBJECT
		if (MME_VARREQ_CONTROLOBJECT_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_CONTROLOBJECT_END) {
			if (sv->ObjectName == _T("(self)")) {
				SetControllObjectParam(sv, node);
			}
			else if (sv->ObjectName == _T("(OffscreenOwner)")) {
				//LN_THROW_NotImpl( 0 );
			}
			else {
				// シーン単位のパラメータ設定で設定済み
			}
			continue;
		}

		ShaderVariable* var = sv->Variable;

		// 正確な範囲じゃないけど、少しでも条件判定の回数を減らすために
		if (MME_VARREQ_GEOMETRY_MATRIX_BEGIN <= sv->Request && sv->Request <= MME_VARREQ_GEOMETRY_MATRIX_END)
		{
			// 「float4x4 WorldLightViewMatrix : WORLDVIEWPROJECTION < string Object = "Light"; > ;」 等、
			// ライトの行列とノードの行列を乗算したものが必要である場合
			if (sv->LightNum > 0 && node->GetAffectLightList() != NULL)
			{
				bool isSet = false;
				if (node->GetAffectLightList()->GetCount() > 0)
				{
					// 影響ライトの 0 番
					Light* light = node->GetAffectLightList()->GetAt(0);
					switch (sv->Request)	// ライト単体の座標変換行列
					{
						// 行列
					case MME_VARREQ_MATRIX_LIGHT_View:
						var->SetMatrix(light->GetViewMatrix()/* * params.Mat_LightView*/);
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj:
						var->SetMatrix(light->GetProjectionMatrix());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj:
						var->SetMatrix(light->GetViewProjectionMatrix());
						break;

						// 逆行列
					case MME_VARREQ_MATRIX_LIGHT_View_I:
						var->SetMatrix(light->GetViewMatrixI());
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj_I:
						var->SetMatrix(light->GetProjectionMatrixI());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj_I:
						var->SetMatrix(light->GetViewProjectionMatrixI());
						break;

						// 転置行列
					case MME_VARREQ_MATRIX_LIGHT_View_T:
						var->SetMatrix(light->GetViewMatrixT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj_T:
						var->SetMatrix(light->GetProjectionMatrixT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj_T:
						var->SetMatrix(light->GetViewProjectionMatrixT());
						break;

						// 逆行列×転置行列
					case MME_VARREQ_MATRIX_LIGHT_View_IT:
						var->SetMatrix(light->GetViewMatrixIT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_Proj_IT:
						var->SetMatrix(light->GetProjectionMatrixIT());
						break;
					case MME_VARREQ_MATRIX_LIGHT_ViewProj_IT:
						var->SetMatrix(light->GetViewProjectionMatrixIT());
						break;

						//	// ライトの色
						//case MME_VARREQ_LIGHT_DIFFUSE:
						//	var->SetVector(*params.LightDiffuse);
						//	break;
						//case MME_VARREQ_LIGHT_AMBIENT:
						//	var->SetVector(*params.LightAmbient);
						//	break;
						//case MME_VARREQ_LIGHT_SPECULAR:
						//	var->SetVector(*params.LightSpecular);
						//	break;

						//	// ライトの位置・向き
						//case MME_VARREQ_LIGHT_POSITION:
						//	var->SetVector(params.LightPosition);
						//	break;
						//case MME_VARREQ_LIGHT_DIRECTION:
						//	var->SetVector(params.LightDirection);
						//	break;
					}
				}

				if (!isSet)
				{
					// SetMatrixArray() で設定する必要がある項目
					if (sv->LightParamIsMatrix && sv->LightParamIsArray)
					{
						Matrix matrices[MaxAffectLights];
						for (int i = 0; i < sv->LightNum; ++i) {
							GetGeometryTransform(node, affectCamera, affectLightList, sv->Request, i, &matrices[i]);
						}
						sv->Variable->SetMatrixArray(matrices, sv->LightNum);
					}
					// SetMatrix() で設定する必要がある項目
					else if (sv->LightParamIsMatrix && !sv->LightParamIsArray)
					{
						Matrix mat;
						GetGeometryTransform(node, affectCamera, affectLightList, sv->Request, 0, &mat);
						sv->Variable->SetMatrix(mat);
					}
					// SetVectorArray() で設定する必要がある項目
					else if (!sv->LightParamIsMatrix && sv->LightParamIsArray)
					{
						LightNodeList* lights = node->GetAffectLightList();
						Vector4 vectors[MaxAffectLights];
						switch (sv->Request)
						{
						case MME_VARREQ_LIGHT_DIFFUSE:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = static_cast< const Vector4& >(lights->GetAt(i)->GetDiffuseColor());
							}
							break;
						case MME_VARREQ_LIGHT_AMBIENT:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = static_cast< const Vector4& >(lights->GetAt(i)->GetAmbientColor());
							}
							break;
						case MME_VARREQ_LIGHT_SPECULAR:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = static_cast< const Vector4& >(lights->GetAt(i)->GetSpecularColor());
							}
							break;
						case MME_VARREQ_LIGHT_POSITION:
							for (int i = 0; i < sv->LightNum; ++i) {
								const Vector3& v = lights->GetAt(i)->GetPosition();
								vectors[i].Set(v.x, v.y, v.z, 0.0f);
							}
							break;
						case MME_VARREQ_LIGHT_DIRECTION:
							for (int i = 0; i < sv->LightNum; ++i) {
								vectors[i] = lights->GetAt(i)->GetDirectionInternal();
							}
							break;
						}

						sv->Variable->SetVectorArray(vectors, sv->LightNum);
					}
					// 行列型、配列型ではない (SetVector())
					else
					{
						// 影響ライトの 0 番
						Light* light = node->GetAffectLightList()->GetAt(0);
						switch (sv->Request)
						{
						case MME_VARREQ_LIGHT_DIFFUSE:
							sv->Variable->SetVector(static_cast< const Vector4& >(light->GetDiffuseColor()));
							break;
						case MME_VARREQ_LIGHT_AMBIENT:
							sv->Variable->SetVector(static_cast< const Vector4& >(light->GetAmbientColor()));
							break;
						case MME_VARREQ_LIGHT_SPECULAR:
							sv->Variable->SetVector(static_cast< const Vector4& >(light->GetSpecularColor()));
							break;
						case MME_VARREQ_LIGHT_POSITION:
						{
							const Vector3& v = light->GetPosition();
							Vector4 vec(v, 0.0f);
							sv->Variable->SetVector(vec);
							break;
						}
						case MME_VARREQ_LIGHT_DIRECTION:
						{
							sv->Variable->SetVector(light->GetDirectionInternal());
							break;
						}
						}
					}
				}
			}
			// ライト以外
			else
			{
				Matrix mat;
				if (GetGeometryTransform(node, affectCamera, affectLightList, sv->Request, 0, &mat)) {
					sv->Variable->SetMatrix(mat);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateSubsetParams(const Internal::VisualNodeSubsetParams& params)
{
    ShaderVariable* var;
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
        var = sv->Variable;

		switch (sv->Request)
		{
			// ディフューズ色（拡散光）
		case MME_VARREQ_OBJECT_DIFFUSE:
			var->SetVector(params.Material.Diffuse);
			break;

			// アンビエント色（環境光）
		case MME_VARREQ_OBJECT_AMBIENT:
			var->SetVector(params.Material.Ambient);
			break;

			// エミッション色（放射光）
		case MME_VARREQ_OBJECT_EMISSIVE:
			var->SetVector(params.Material.Emissive);
			break;

			// スペキュラ色（鏡面光）
		case MME_VARREQ_OBJECT_SPECULAR:
			var->SetVector(params.Material.Specular);
			break;

			// スペキュラの強度
		case MME_VARREQ_OBJECT_SPECULARPOWER:
			var->SetFloat(params.Material.Power);
			break;

			// トゥーン色
		case MME_VARREQ_OBJECT_TOONCOLOR:
			var->SetVector(params.Material.ToonColor);
			break;

			// 輪郭色
		case MME_VARREQ_OBJECT_EDGECOLOR:
			var->SetVector(params.Material.EdgeColor);
			break;

			// マテリアルに設定されているテクスチャ
		case MME_VARREQ_OBJECT_MATERIALTEXTURE:
			if (!params.Material.Texture.IsNull()) {
				var->SetTexture(params.Material.Texture);
			}
			// テクスチャがなければダミーを設定
			else {
				var->SetTexture(m_manager->GetDummyTexture());
			}
			break;

			// マテリアルに設定されている、スフィアマップテクスチャ
		case MME_VARREQ_OBJECT_MATERIALSPHEREMAP:
			if (!params.Material.SphereTexture.IsNull()) {
				var->SetTexture(params.Material.SphereTexture);
			}
			// テクスチャがなければダミーを設定
			else {
				var->SetTexture(m_manager->GetDummyTexture());
			}
			break;

			// 不透明度
		case LN_VARREQ_OPACITY:
		{
			var->SetFloat(params.Opacity);
			break;
		}
			// 乗算する色
		case LN_VARREQ_COLOR_SCALE:
		{
			var->SetVector(params.ColorScale);
			break;
		}
			// ブレンドする色
		case LN_VARREQ_BLEND_COLOR:
		{
			var->SetVector(params.BlendColor);
			break;
		}
			// 色調
		case LN_VARREQ_TONE:
		{
			var->SetVector(params.Tone);
			break;
		}
			// テクスチャ座標変換行列
		case LN_VARREQ_UVTRANSFORM:
		{
			var->SetMatrix(params.UVTransform);
			break;
		}
		}
    }
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::UpdateSubsetParams(const detail::MaterialInstance& material)
{
	LN_CHECK_ARGS_RETURN(material.m_owner->GetMaterialTypeId() == detail::MmdMaterialTypeId);

	MmdMaterial* ownerMaterial = static_cast<MmdMaterial*>(material.m_owner);

	ShaderVariable* var;
	LN_FOREACH(MMEShaderVariable* sv, m_mmeShaderVariableList)
	{
		var = sv->Variable;

		switch (sv->Request)
		{
			case MME_VARREQ_OBJECT_DIFFUSE:
				var->SetVector(ownerMaterial->m_diffuse);
				break;
			case MME_VARREQ_OBJECT_AMBIENT:
				var->SetVector(ownerMaterial->m_ambient);
				break;
			case MME_VARREQ_OBJECT_EMISSIVE:
				var->SetVector(ownerMaterial->m_emissive);
				break;
			case MME_VARREQ_OBJECT_SPECULAR:
				var->SetVector(ownerMaterial->m_specular);
				break;
			case MME_VARREQ_OBJECT_SPECULARPOWER:
				var->SetFloat(ownerMaterial->m_power);
				break;
			case MME_VARREQ_OBJECT_TOONCOLOR:
				var->SetVector(ownerMaterial->ToonColor);
				break;
			case MME_VARREQ_OBJECT_EDGECOLOR:
				var->SetVector(ownerMaterial->EdgeColor);
				break;
			case MME_VARREQ_OBJECT_MATERIALTEXTURE:
				var->SetTexture((ownerMaterial->GetTexture() != nullptr) ? ownerMaterial->GetTexture() : m_manager->GetDummyTexture());	// テクスチャがなければダミーを設定
				break;
			case MME_VARREQ_OBJECT_MATERIALSPHEREMAP:
				var->SetTexture((ownerMaterial->m_sphereTexture != nullptr) ? ownerMaterial->m_sphereTexture : m_manager->GetDummyTexture());	// テクスチャがなければダミーを設定
				break;
			case LN_VARREQ_OPACITY:
				var->SetFloat(1);	// TODO: 廃止予定
				break;
			case LN_VARREQ_COLOR_SCALE:
				var->SetVector(material.m_colorScale);
				break;
			case LN_VARREQ_BLEND_COLOR:
				var->SetVector(material.m_blendColor);
				break;
			case LN_VARREQ_TONE:
				var->SetVector(material.m_tone);
				break;
			case LN_VARREQ_UVTRANSFORM:
				var->SetMatrix(material.GetUVTransform());
				break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MMEShader::SetControllObjectParam(MMEShaderVariable* sv, const IMMESceneObject* obj)
{
	ShaderVariable* var = sv->Variable;

	if (sv->Request == MME_VARREQ_CONTROLOBJECT_Visible)
	{
		if (obj == NULL) {
			var->SetBool(false);
		}
		else {
			var->SetBool(obj->IsVisible());
		}
	}
	else if (obj != NULL)
	{
		switch (sv->Request)
		{
		case MME_VARREQ_CONTROLOBJECT_Scale:
		{
			Vector4 scale;
			obj->GetMatrix().Decompose((Vector3*)&scale, NULL, NULL);
			var->SetVector(scale);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Position:
			var->SetVector((const Vector4&)obj->GetMatrix().GetPosition());
			break;
		case MME_VARREQ_CONTROLOBJECT_World:
			var->SetMatrix(obj->GetMatrix());
			break;

		case MME_VARREQ_CONTROLOBJECT_BoneOffset:
			var->SetVector(
				(const Vector4&)obj->FindFrameMarix(sv->ItemName).GetPosition());
			break;
		case MME_VARREQ_CONTROLOBJECT_BoneMatrix:
			var->SetMatrix(
				obj->FindFrameMarix(sv->ItemName));
			break;
		case MME_VARREQ_CONTROLOBJECT_MorphBlend:
			var->SetFloat(obj->FindMorphBlend(sv->ItemName));
			break;

		case MME_VARREQ_CONTROLOBJECT_X:
			var->SetFloat(obj->GetMatrix().GetPosition().x);
			break;
		case MME_VARREQ_CONTROLOBJECT_Y:
			var->SetFloat(obj->GetMatrix().GetPosition().y);
			break;
		case MME_VARREQ_CONTROLOBJECT_Z:
			var->SetFloat(obj->GetMatrix().GetPosition().z);
			break;
		case MME_VARREQ_CONTROLOBJECT_XYZ:
			var->SetVector((const Vector4&)obj->GetMatrix().GetPosition());
			break;

		case MME_VARREQ_CONTROLOBJECT_Rx:
		{
			Vector3 rad = obj->GetMatrix().ToEulerAngles();
			var->SetFloat(rad.x);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Ry:
		{
			Vector3 rad = obj->GetMatrix().ToEulerAngles();
			var->SetFloat(rad.y);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Rz:
		{
			Vector3 rad = obj->GetMatrix().ToEulerAngles();
			var->SetFloat(rad.z);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Rxyz:
		{
			Vector4 rad(obj->GetMatrix().ToEulerAngles(), 0);
			var->SetVector(rad);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Si:
		{
			Vector4 scale;
			obj->GetMatrix().Decompose((Vector3*)&scale, NULL, NULL);
			var->SetFloat(scale.x);
			break;
		}
		case MME_VARREQ_CONTROLOBJECT_Tr:
			var->SetFloat(obj->GetOpacity());
			break;
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool MMEShader::GetGeometryTransform(SceneNode* node, Camera* affectCamera, const LightNodeList& affectLightList, MMEVariableRequest req, int lightIndex, Matrix* outMatrix)
{
	switch (req)
	{
	case MME_VARREQ_MATRIX_World:
	{
		*outMatrix = node->GetMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		return true;
	}

		// 逆行列
	case MME_VARREQ_MATRIX_World_I:
	{
		*outMatrix = Matrix::Inverse(node->GetMatrix());
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView_I:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		outMatrix->Inverse();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_I:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		outMatrix->Inverse();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView_I:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		outMatrix->Inverse();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_I:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		outMatrix->Inverse();
		return true;
	}

		// 転置行列
	case MME_VARREQ_MATRIX_World_T:
	{
		*outMatrix = Matrix::Transpose(node->GetMatrix());
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView_T:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_T:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView_T:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_T:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		outMatrix->Transpose();
		return true;
	}

		// 逆行列×転置行列
	case MME_VARREQ_MATRIX_World_IT:
	{
		*outMatrix = Matrix::Inverse(node->GetMatrix());
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldView_IT:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_CAMERA_WorldViewProj_IT:
	{
		*outMatrix = node->GetMatrix() * affectCamera->GetViewProjectionMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldView_IT:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	case MME_VARREQ_MATRIX_LIGHT_WorldViewProj_IT:
	{
		*outMatrix = node->GetMatrix() * affectLightList[lightIndex]->GetViewProjectionMatrix();
		outMatrix->Inverse();
		outMatrix->Transpose();
		return true;
	}
	}

	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
MMEShaderTechnique* MMEShader::FindTechnique(MMDPass mmdPass, bool UseTexture, bool UseSphereMap, bool UseToon, bool UseSelfShadow, int subsetIndex)
{
	/* MMM では複数のテクニックがマッチしても、実際に使われるのは
	 * ファイル上で先に記述されている 1 つだけ。
	 */

	LN_FOREACH(MMEShaderTechnique* tech, m_mmeShaderTechniqueList)
	{
		if (tech->GetMMDPass() != mmdPass) {
			continue;
		}
		if (!tech->IsUseTexture().IsNull() && tech->IsUseTexture() != UseTexture) {	// 省略されている場合はマッチしているとする
			continue;
		}
		if (!tech->IsUseSphereMap().IsNull() && tech->IsUseSphereMap() != UseSphereMap) {	// 省略されている場合はマッチしているとする
			continue;
		}
		if (!tech->IsUseToon().IsNull() && tech->IsUseToon() != UseToon) {	// 省略されている場合はマッチしているとする
			continue;
		}
		if (!tech->IsUseSelfShadow().IsNull() && tech->IsUseSelfShadow() != UseSelfShadow) {	// 省略されている場合はマッチしているとする
			continue;
		}
		if (!tech->ContainsSubsetIndex(subsetIndex)) {
			continue;
		}
		return tech;
	}
	return NULL;
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

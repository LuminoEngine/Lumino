
#pragma once
#include "../Graphics/Color.h"
#include "../Graphics/Rendering.h"	// for DynamicLightInfo
#include "SceneNode.h"
#include "WorldObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/// LightComponent
class LightComponent
	: public SceneNode
{
public:
	LightComponent();
	virtual ~LightComponent();
	void Initialize(SceneGraph* owner, LightType type);

public:

	/// ライトの種類の取得
	LightType GetType() const { return m_lightInfo->m_type; }

	/// ライトの有効、無効を設定する
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	/// ライトの有効、無効を確認する
	bool IsEnabled() const { return m_enabled; }

	/// ディフューズカラーの設定
	void SetDiffuseColor(const Color& color) { m_lightInfo->m_diffuse = color; }

	/// ディフューズカラーの取得
	const Color& GetDiffuseColor() const { return m_lightInfo->m_diffuse; }

	/// アンビエントカラーの設定
	void SetAmbientColor(const Color& color) { m_lightInfo->m_ambient = color; }

	/// アンビエントカラーの取得
	const Color& GetAmbientColor() const { return m_lightInfo->m_ambient; }

	/// スペキュラカラーの設定
	void SetSpecularColor(const Color& color) { m_lightInfo->m_specular = color; }

	/// スペキュラカラーの取得
	const Color& GetSpecularColor() const { return m_lightInfo->m_specular; }

	/// スポットライトのコーン角度の設定 (ラジアン単位)
	void SetSpotAngle(float angle) { m_spotAngle = angle; }

	/// スポットライトのコーン角度の取得 (ラジアン単位)
	float GetSpotAngle() const { return m_spotAngle; }

	/// [Advanced]
	void SetShadowZFar(float z) { m_lightInfo->m_shadowZFar = z; }

	/// [Advanced]
	float GetShadowZFar() const { return m_lightInfo->m_shadowZFar; }

public:	// internal

	/// 各行列を更新する (SceneNode::UpdateFrameHierarchy() の後で呼び出すこと)
	void UpdateMatrices(/*const Size& viewSize*/);

	// 向きの取得 (シェーダ設定用。UpdateMatrices() の後で呼び出すこと)
	//const Vector4& GetDirectionInternal() const { return m_lightInfo->m_direction; }

	// 行列の取得 (シェーダ設定用。UpdateMatrices() の後で呼び出すこと)
	//const Matrix& GetWorldViewProj() const { return m_worldViewProjMatrix; }
	//const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	//const Matrix& GetProjectionMatrix() const { return m_projMatrix; }
	//const Matrix& GetViewProjectionMatrix() const { return m_viewProjMatrix; }
	//const Matrix& GetViewMatrixI() const { return m_viewMatrixI; }
	//const Matrix& GetProjectionMatrixI() const { return m_projMatrixI; }
	//const Matrix& GetViewProjectionMatrixI() const { return m_viewProjMatrixI; }
	//const Matrix& GetViewMatrixT() const { return m_viewMatrixT; }
	//const Matrix& GetProjectionMatrixT() const { return m_projMatrixT; }
	//const Matrix& GetViewProjectionMatrixT() const { return m_viewProjMatrixT; }
	//const Matrix& GetViewMatrixIT() const { return m_viewMatrixIT; }
	//const Matrix& GetProjectionMatrixIT() const { return m_projMatrixIT; }
	//const Matrix& GetViewProjectionMatrixIT() const { return m_viewProjMatrixIT; }

protected:
	virtual void OnRender(DrawList* renderer) override;

private:
	friend class VisualComponent;

	//LightType			m_type;				///< ライトの種類
	//Color	m_diffuse;			///< ディフューズカラー
	//Color	m_ambient;			///< アンビエントカラー
	//Color	m_specular;			///< スペキュラカラー
	RefPtr<detail::DynamicLightInfo>	m_lightInfo;
	bool				m_enabled;			///< 有効状態
	float				m_spotAngle;		///< コーン角度 (ラジアン単位)

	Matrix				m_viewMatrix;		///< ビュー行列
	//Matrix				m_projMatrix;		///< プロジェクション行列
	//Matrix				m_viewProjMatrix;	///< ビュー行列とプロジェクション行列の積
	//Vector4				m_direction;		///< 向き

	// 以下はシェーダ変数への設定用。ライトは個々のノードに比べて参照される回数が多いので
	// 必要になるたびに計算するのではなく、あらかじめ計算しておく。
	//Matrix				m_worldViewProjMatrix;
	//Matrix				m_viewMatrixI;		///< ビュー行列 (Inverse)
	//Matrix				m_projMatrixI;		///< プロジェクション行列 (Inverse)
	//Matrix				m_viewProjMatrixI;	///< ビュー行列とプロジェクション行列の積 (Inverse)
	//Matrix				m_viewMatrixT;		///< ビュー行列 (Transpose)
	//Matrix				m_projMatrixT;		///< プロジェクション行列 (Transpose)
	//Matrix				m_viewProjMatrixT;	///< ビュー行列とプロジェクション行列の積 (Transpose)
	//Matrix				m_viewMatrixIT;		///< ビュー行列 (Inverse * Transpose)
	//Matrix				m_projMatrixIT;		///< プロジェクション行列 (Inverse * Transpose)
	//Matrix				m_viewProjMatrixIT;	///< ビュー行列とプロジェクション行列の積 (Inverse * Transpose)

	float				m_tmpDistance;		///< ノードに近いライトを検索するときの作業用変数
};


/**
	@brief
*/
class Light
	: public WorldObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	LightComponent* GetLightComponent() const;

LN_CONSTRUCT_ACCESS:
	Light();
	virtual ~Light();
	void Initialize(SceneGraph* owner);

LN_INTERNAL_ACCESS:

private:
	RefPtr<LightComponent>	m_component;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

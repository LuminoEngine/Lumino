
#pragma once
#include "SceneNode.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

/// Camera
class Camera
	: public SceneNode
{
public:

	/// Y 方向視野角の設定
	void SetFovY(float fov_y) { m_fovY = fov_y; }

	/// Y 方向視野角の取得
	float GetFovY() const { return m_fovY; }

	/// 最も近いビュープレーン位置の設定
	void SetNearClip(float nearClip) { m_nearClip = nearClip; }

	/// 最も近いビュープレーン位置の取得
	float GetNearClip() const { return m_nearClip; }

	/// 最も遠いビュープレーン位置の設定
	void SetFarClip(float farClip) { m_farClip = farClip; }

	/// 最も遠いビュープレーン位置の取得
	float GetFarClip() const { return m_farClip; }

public:	// internal

	/// 各行列を更新する (SceneNode::UpdateFrameHierarchy() の後で呼び出すこと)
	void UpdateMatrices(const SizeF& viewSize);

	// 向きの取得 (シェーダ設定用。UpdateMatrices() の後で呼び出すこと)
	const Vector4& GetDirectionInternal() const { return m_direction; }

	// 行列の取得 (シェーダ設定用。UpdateMatrices() の後で呼び出すこと)
	const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	const Matrix& GetProjectionMatrix() const { return m_projMatrix; }
	const Matrix& GetViewProjectionMatrix() const { return m_viewProjMatrix; }
	const Matrix& GetViewMatrixI() const { return m_viewMatrixI; }
	const Matrix& GetProjectionMatrixI() const { return m_projMatrixI; }
	const Matrix& GetViewProjectionMatrixI() const { return m_viewProjMatrixI; }
	const Matrix& GetViewMatrixT() const { return m_viewMatrixT; }
	const Matrix& GetProjectionMatrixT() const { return m_projMatrixT; }
	const Matrix& GetViewProjectionMatrixT() const { return m_viewProjMatrixT; }
	const Matrix& GetViewMatrixIT() const { return m_viewMatrixIT; }
	const Matrix& GetProjectionMatrixIT() const { return m_projMatrixIT; }
	const Matrix& GetViewProjectionMatrixIT() const { return m_viewProjMatrixIT; }

protected:
	Camera(SceneGraphManager* manager, CameraProjection proj);
	virtual ~Camera();

private:

	CameraProjection	m_projectionMode;
	float				m_fovY;
	float				m_nearClip;
	float				m_farClip;

	Matrix				m_viewMatrix;		///< ビュー行列
	Matrix				m_projMatrix;		///< プロジェクション行列
	Matrix				m_viewProjMatrix;	///< ビュー行列とプロジェクション行列の積
	Vector4				m_direction;		///< 向き

	// 以下はシェーダ変数への設定用。ライトは個々のノードに比べて参照される回数が多いので
	// 必要になるたびに計算するのではなく、あらかじめ計算しておく。
	Matrix				m_viewMatrixI;		///< ビュー行列 (Inverse)
	Matrix				m_projMatrixI;		///< プロジェクション行列 (Inverse)
	Matrix				m_viewProjMatrixI;	///< ビュー行列とプロジェクション行列の積 (Inverse)
	Matrix				m_viewMatrixT;		///< ビュー行列 (Transpose)
	Matrix				m_projMatrixT;		///< プロジェクション行列 (Transpose)
	Matrix				m_viewProjMatrixT;	///< ビュー行列とプロジェクション行列の積 (Transpose)
	Matrix				m_viewMatrixIT;		///< ビュー行列 (Inverse * Transpose)
	Matrix				m_projMatrixIT;		///< プロジェクション行列 (Inverse * Transpose)
	Matrix				m_viewProjMatrixIT;	///< ビュー行列とプロジェクション行列の積 (Inverse * Transpose)

	friend class Internal::SceneHelper;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

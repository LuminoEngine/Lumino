
#pragma once

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

class IMMESceneObject
{
public:
	IMMESceneObject() {}
	virtual ~IMMESceneObject() {}

public:

	// CONTROLOBJECT アノテーション関係
	virtual bool IsVisible() const = 0;
	virtual const Matrix& GetMatrix() const = 0;
	virtual float GetOpacity() const = 0;
	virtual const Matrix& FindFrameMarix(const String& name) const = 0;
	virtual float FindMorphBlend(const String& name) const = 0;


	// オブジェクトのワールド行列と、カメラまたはライトのビュー・プロジェクション行列を求める。
	// out に値を格納した場合は true を返す。
	//virtual bool GetObjectMatrix(Matrix* out, MMEVariableRequest req, int lightIndex) const = 0;
	// TODO: ↓のはどのみち必要。なら、↑のは static にできない？
	virtual LightNodeList* GetAffectLightList() = 0;
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino

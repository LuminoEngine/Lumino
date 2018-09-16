#pragma once
#include <Lumino/Shader/Shader.hpp>

namespace ln {
namespace detail {

class SceneRendererPass
	: public RefObject
{
public:
	// Element の情報と派生 Pass から、最終的に使いたい ShaderTechnique を求める
	virtual ShaderTechnique* selectShaderTechnique(
		ShaderTechniqueClass_MeshProcess requestedMeshProcess,
		Shader* requestedShader,
		ShadingModel requestedShadingModel) = 0;

private:
};

class SceneRenderer
	: public RefObject
{
public:

private:
};

} // namespace detail
} // namespace ln



#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/String.h>

LN_NAMESPACE_BEGIN
class Texture2D;
class Shader;
class StaticMeshModel;

/**
	@brief
*/
class Assets
{
public:
	static const String standardContentsDirectory;

	static void addAssetsDirectory(const StringRef& directoryPath);

	/** アセットフォルダからテクスチャを読み込みます。 */
	static Ref<Texture2D> loadTexture(const StringRef& filePath);

	/** アセットフォルダからシェーダを読み込みます。 */
	static Ref<Shader> loadShader(const StringRef& filePath);

	static Ref<StaticMeshModel> loadMeshModel(const StringRef& filePath);

	static String loadText(const StringRef& filePath);

};

LN_NAMESPACE_END

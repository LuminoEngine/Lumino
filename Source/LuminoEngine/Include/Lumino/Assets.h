
#pragma once
#include <Lumino/Base/RefObject.h>

LN_NAMESPACE_BEGIN
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
	static Texture2DPtr loadTexture(const StringRef& filePath);

	/** アセットフォルダからシェーダを読み込みます。 */
	static Ref<Shader> loadShader(const StringRef& filePath);

	static Ref<StaticMeshModel> loadMeshModel(const StringRef& filePath);

	static String loadText(const StringRef& filePath);

};

LN_NAMESPACE_END

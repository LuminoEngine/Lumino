
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

	static void addAssetsDirectory(const StringRef& directoryPath);

	static Texture2DPtr loadTexture(const StringRef& filePath);

	static RefPtr<StaticMeshModel> loadMeshModel(const StringRef& filePath);

	static String loadText(const StringRef& filePath);

};

LN_NAMESPACE_END

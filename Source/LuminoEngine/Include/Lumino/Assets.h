
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

	static void AddAssetsDirectory(const StringRef& directoryPath);

	static Texture2DPtr LoadTexture(const StringRef& filePath);

	static RefPtr<StaticMeshModel> LoadMeshModel(const StringRef& filePath);

	static String LoadText(const StringRef& filePath);

};

LN_NAMESPACE_END

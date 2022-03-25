#pragma once
#include "../Asset/Common.hpp"

namespace ln {

struct RuntimeModuleSettings {
    AssetStorageAccessPriority assetStorageAccessPriority = AssetStorageAccessPriority::DirectoryFirst;
};

class Runtime {
public:
    /**
	 * Initialize Runtime.
	 */
    static Result initialize(const RuntimeModuleSettings& settings = RuntimeModuleSettings{});

    /**
	 * terminate Runtime.
	 */
    static void terminate();

    
    static void mountAssetDirectory(const StringView& path);

    static void mountAssetArchive(const StringView& filePath, const StringView& password = StringView());
};

} // namespace ln

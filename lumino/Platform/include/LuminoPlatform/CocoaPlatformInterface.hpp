#pragma once

namespace ln {

class CocoaPlatformInterface
{
public:
	static int Main();
	static void addAssetArchive(const StringRef& fileFullPath, const StringRef& password = StringRef());
};

} // namespace ln


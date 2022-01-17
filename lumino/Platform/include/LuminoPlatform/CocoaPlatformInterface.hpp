#pragma once

namespace ln {

class CocoaPlatformInterface
{
public:
	static int Main();
	static void addAssetArchive(const StringView& fileFullPath, const StringView& password = StringView());
};

} // namespace ln


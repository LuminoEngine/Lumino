#pragma once

namespace ln {

class BuildAssetHelper
{
public:
	static ln::MaybeResult buildShader(const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir);
};

} // namespace lna




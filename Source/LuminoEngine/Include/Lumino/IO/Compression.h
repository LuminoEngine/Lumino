
#pragma once

LN_NAMESPACE_BEGIN

class ZipFile
{
public:
	static void ExtractToDirectory(const Path& sourceArchiveFilePath, const Path& destinationDirectoryPath);
};

LN_NAMESPACE_END

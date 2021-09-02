
#include <Project/Project.hpp>
#include <Project/BuildAssetHelper.hpp>
#include "FxcCommand.hpp"

int FxcCommand::execute(const ln::Path& inputFile)
{
	auto result = lna::BuildAssetHelper::buildShader(inputFile, outputFile, exportDir);
	return result ? 0 : 1;
}

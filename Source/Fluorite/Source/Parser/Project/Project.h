
#pragma once
#include "../Common.h"
#include "InputFile.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
/**
	@brief	解析対象ファイルの集合
	@note	もしかしたらいらないかもしれないけど、頭の整理のために用意。
*/
class Project
{
public:
	Array<InputFilePtr>		inputFileList;

	Project() = default;
	~Project() = default;

	void AddInputFile(const PathName& filePath);
};

} // namespace Parser
LN_NAMESPACE_END


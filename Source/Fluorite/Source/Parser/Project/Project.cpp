
#include "../Internal.h"
#include "Project.h"

LN_NAMESPACE_BEGIN
namespace parser
{

//=============================================================================
// Project
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Project::AddInputFile(const PathName& filePath)
{
	auto file = InputFilePtr::MakeRef();
	file->filePath = filePath;
	inputFileList.Add(file);
}

} // namespace Parser
LN_NAMESPACE_END


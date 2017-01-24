
#pragma once
#include "../Common.h"
#include "InputFileSettings.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
/**
	@brief	‰ğÍ‘ÎÛƒtƒ@ƒCƒ‹
*/
class InputFile
	: public RefObject
{
public:
	PathName			filePath;
	InputFileSettings	settings;
};
typedef RefPtr<InputFile>	InputFilePtr;

} // namespace Parser
LN_NAMESPACE_END


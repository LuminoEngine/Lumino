
#pragma once

#include "MMETypes.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

enum MMEShaderError
{
	MMEShaderError_Success = 0,

	MMEShaderError_ScriptCommand_InvalidRenderTargetVariable,				///< "RenderColorTarget" コマンドでテクスチャ型変数が見つからなかった
	MMEShaderError_ScriptCommand_InvalidRenderDepthStencilTargetVariable,	///< "RenderDepthStencilTarget" コマンドでテクスチャ型変数が見つからなかった
	MMEShaderError_ScriptCommand_InvalidClearSetColorVariable,				///< "ClearSetColor" コマンドで Vector 型変数が見つからなかった
	MMEShaderError_ScriptCommand_InvalidClearSetDepthVariable,				///< "ClearSetDepth" コマンドで Vector 型変数が見つからなかった
	MMEShaderError_ScriptCommand_InvalidPassName,							///< "Pass" コマンドで有効なパス名が見つからなかった
	MMEShaderError_ScriptCommand_InvalidLoopByCountVariable,				///< "LoopByCount" コマンドで Int 型変数が見つからなかった
	MMEShaderError_ScriptCommand_InvalidLoopGetIndexVariable,				///< "LoopGetIndex" コマンドで Int 型変数が見つからなかった
};

/// MMEShaderErrorObject
class MMEShaderErrorObject
{
public:
	MMEShaderError	ErrorCode;
	String			Message;
};
	
/// MMEShaderErrorInfo
class MMEShaderErrorInfo
{
public:
	void AddError(MMEShaderError errorCode);
	void AddError(MMEShaderError errorCode, const String& message);

private:
	Array<MMEShaderErrorObject>	m_errorList;
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino

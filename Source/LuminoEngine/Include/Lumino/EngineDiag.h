
#pragma once

LN_NAMESPACE_BEGIN

/** 診断情報の表示モード */
enum class EngineDiagDisplayMode
{
	Hide = 0,			/**< 表示しない */
	FpsSummary = 1,
	InPlaceDetails = 2,
};

/**
	@brief	
*/
class EngineDiag
{
public:

	static void SetDisplayMode(EngineDiagDisplayMode mode);

	static int GetGraphicsDeviceDrawCount();

	static int GetVisualNodeDrawCount();
};

LN_NAMESPACE_END

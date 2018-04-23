#pragma once

namespace ln {

class LN_API UIFrameWindow
	: public RefObject
{
public:
	UIFrameWindow();
	virtual ~UIFrameWindow();

	void initialize();
	void dispose();
};

} // namespace ln


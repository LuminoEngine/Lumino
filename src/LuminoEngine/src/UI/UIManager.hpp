#pragma once

namespace ln {
namespace detail {

class UIManager
	: public RefObject
{
public:
	UIManager();
	virtual ~UIManager() = default;

	void initialize();
	void dispose();
};

} // namespace detail
} // namespace ln


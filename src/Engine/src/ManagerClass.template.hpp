#pragma once

namespace ln {
namespace detail {

class Manager
	: public RefObject
{
public:
	Manager();
	virtual ~Manager();

	void initialize();
	void dispose();
};

} // namespace detail
} // namespace ln


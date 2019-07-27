
#pragma once

namespace ln {
namespace detail { class PlatformWindow; }


class PlatformFileOpenDialog
	: public Object
{
public:
	static Ref<PlatformFileOpenDialog> create();

public:
	virtual bool showDialog(detail::PlatformWindow* parent) = 0;

	virtual Path getFilePath() = 0;

protected:
	PlatformFileOpenDialog();
	virtual ~PlatformFileOpenDialog();
};

} // namespace ln


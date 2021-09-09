
#pragma once

namespace ln {
namespace detail { class PlatformWindow; }


class PlatformOpenFileDialog
	: public RefObject
{
public:
	static Ref<PlatformOpenFileDialog> create();

	virtual bool showDialog(detail::PlatformWindow* parent) = 0;
	virtual Path getPath() = 0;

protected:
	virtual ~PlatformOpenFileDialog();
};

class PlatformSelectFolderDialog
    : public RefObject
{
public:
    static Ref<PlatformSelectFolderDialog> create();

    virtual bool showDialog(detail::PlatformWindow* parent) = 0;
    virtual Path getPath() = 0;

protected:
    virtual ~PlatformSelectFolderDialog();
};

} // namespace ln


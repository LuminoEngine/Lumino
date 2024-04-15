
#pragma once
#include "Common.hpp"

namespace ln {

class PlatformOpenFileDialog
	: public RefObject
{
public:
	static Ref<PlatformOpenFileDialog> create();

	virtual bool showDialog(PlatformWindow* parent) = 0;
	virtual Path getPath() = 0;

protected:
	virtual ~PlatformOpenFileDialog();
};

class PlatformSelectFolderDialog
    : public RefObject
{
public:
    static Ref<PlatformSelectFolderDialog> create();

    virtual bool showDialog(PlatformWindow* parent) = 0;
    virtual Path getPath() = 0;

protected:
    virtual ~PlatformSelectFolderDialog();
};

} // namespace ln


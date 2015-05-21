
#pragma once
#include "Common.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief		GUI 機能の管理クラスです。
*/
class GUIManager
	: public RefObject
{
public:
	GUIManager();
	virtual ~GUIManager();

public:
	Workbench* CreateWorkbench();

private:
};

} // namespace GUI
} // namespace Lumino

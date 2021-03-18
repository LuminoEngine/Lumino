#pragma once

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <Shlobj.h>
#endif

#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Base/Result.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>
#include <LuminoEngine/UI/Controls/UITreeView.hpp>
#include <LuminoEngine/Platform/PlatformDialogs.hpp>
#include <LuminoEngine/Tilemap/TilemapComponent.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/Controls/UITreeView.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>
#include <LuminoEngine/UI/Controls/UISplitter.hpp>
#include <LuminoEngine/UI/UITabBar.hpp>
#include <LuminoEngine/UI/UICommand.hpp>
#include <LuminoEngine/UI/UIContext.hpp>
#include <LuminoEngine/UI/Controls/UIListView.hpp>
#include <LuminoEngine/UI/Controls/UITextField.hpp>
#include <LuminoEngine/UI/UIPopup.hpp>
#include <LuminoEngine/UI/UIActiveTimer.hpp>
#include <LuminoEngine/UI/Controls/UIListBox.hpp>
#include <LuminoEngine/UI/Controls/UIDialog.hpp>
#include <LuminoEngine/UI/Controls/UILinkLabel.hpp>

#include "CLI.hpp"

template<class T>
using Ref = ln::Ref<T>;


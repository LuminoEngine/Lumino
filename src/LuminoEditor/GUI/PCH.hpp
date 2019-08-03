#pragma once

#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Base/Event.hpp>
#include <LuminoEngine/Platform/PlatformDialogs.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UITreeView.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>
#include <LuminoEngine/UI/UISplitter.hpp>
#include <LuminoEngine/UI/UITabBar.hpp>
#include <LuminoEngine/UI/UICommand.hpp>

template<class T>
using Ref = ln::Ref<T>;

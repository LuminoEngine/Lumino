#pragma once
#include "Object.hpp"

namespace ln {
namespace detail { class ApplicationHelper; }

class LN_API Application
	: public Object
{
public:
	virtual void onCreate();

	virtual void onStart();

	virtual void onUpdate();

	virtual void onStop();

	virtual void onDestroy();

protected:
	Application();
	virtual ~Application();

	friend class detail::ApplicationHelper;
};

namespace detail {
class ApplicationHelper
{
public:
	static void initialize(Application* app);
	static bool processTick(Application* app);
	static void finalize(Application* app);
};
} // namespace detail
} // namespace ln

#define LUMINO_APP(appClass) \
    extern "C" ::ln::Application* LuminoCreateApplicationInstance() \
    { \
        return new appClass(); \
    }


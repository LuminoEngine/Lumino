#pragma once
#include "Object.hpp"

namespace ln {

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
};

} // namespace ln

#define LUMINO_APP(appClass) \
    extern "C" ::ln::Application* LuminoCreateApplicationInstance() \
    { \
        return new appClass(); \
    }



#pragma once
#include <map>
#include "Common.h"
#include "../Variant.h"

#define LN_GUI_CONTENT_ELEMENT_DECLARE(className) \
public: \
	static const String TypeID; \
	virtual const String& GetTypeID() const { return TypeID; } \
private: \
	friend class GUIManager;\
	static className* internalCreateInstance(GUIManager* manager) \
	{ \
		auto obj = RefPtr<className>(LN_NEW className(manager)); \
		obj.SafeAddRef(); \
		return obj; \
	}

#define LN_GUI_CONTENT_ELEMENT_IMPLEMENT(className) \
	const String className::TypeID(_T(#className))

namespace Lumino
{
namespace GUI
{

class ContentElement
	: public CoreObject
{
public:
	ContentElement(GUIManager* manager);
	virtual ~ContentElement();

private:
	GUIManager*	m_manager;
};


} // namespace GUI
} // namespace Lumino

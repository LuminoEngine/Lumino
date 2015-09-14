
#pragma once
#include <map>
#include "Common.h"
#include "../Variant.h"

#define LN_GUI_CONTENT_ELEMENT_DECLARE(className) \
public: \
	static const String TypeID; \
	virtual const String& GetTypeID() const { return TypeID; } \
private: \
	friend class GUIManagerImpl;\
	static className* internalCreateInstance(GUIManagerImpl* manager) \
	{ \
		auto obj = RefPtr<className>(LN_NEW className(manager)); \
		obj.SafeAddRef(); \
		return obj; \
	}

#define LN_GUI_CONTENT_ELEMENT_IMPLEMENT(className) \
	const String className::TypeID(_T(#className))

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

class ContentElement
	: public CoreObject
{
public:
	ContentElement(GUIManagerImpl* manager);
	virtual ~ContentElement();

private:
	GUIManagerImpl*	m_manager;
};


LN_NAMESPACE_GUI_END
} // namespace Lumino

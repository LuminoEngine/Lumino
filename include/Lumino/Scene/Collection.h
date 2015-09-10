
#pragma once
#include <Lumino/Base/BindingSupport.h>
#include "Common.h"

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN
		
class LayerList
	: public RefObjectList
{
public:
	LN_REF_OBJECT_LIST_DECL(Layer);

	LayerList(ViewPane* owner) : m_owner(owner) {}
	virtual ~LayerList() {}

protected:

	virtual bool OnItemAdding(RefObject* item)
	{
		LN_THROW(static_cast<Layer*>(item)->GetParentPane() == NULL, InvalidOperationException);
	}

	virtual void OnItemAdded(RefObject* item)
	{
	}

	virtual void OnItemRemoved(RefObject* item)
	{
	}

private:
	ViewPane*	m_owner;
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino

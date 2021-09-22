
#include "Internal.hpp"
#include <LuminoEngine/Base/Collection.hpp>

namespace ln {

//==============================================================================
// CollectionObject

CollectionObject::CollectionObject()
{
}

CollectionObject::~CollectionObject()
{
}

void CollectionObject::addNotifyCollectionChangedHandler(INotifyCollectionChangedHandler* handler)
{
	m_handlers.add(handler);
}

void CollectionObject::removeNotifyCollectionChangedHandler(INotifyCollectionChangedHandler* handler)
{
	m_handlers.remove(handler);
}

void CollectionObject::raiseCollectionChanged(const NotifyCollectionChangedEventArgs* args)
{
	for (auto& handler : m_handlers) {
		handler->onNotifyCollectionChanged(args);
	}
}

} // namespace ln


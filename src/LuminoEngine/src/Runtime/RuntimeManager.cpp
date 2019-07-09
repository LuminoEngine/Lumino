#include "RuntimeManager.hpp"

namespace ln {
namespace detail {

RuntimeManager::RuntimeManager()
	: m_systemAliving(false)
{
}

RuntimeManager::~RuntimeManager()
{
}

void RuntimeManager::init(const Settings& settings)
{
	// オブジェクト管理配列
	for (int i = 511; i >= 0; --i)
	{
		ObjectEntry e;
		e.object = nullptr;
		e.index = i;
		m_objectEntryList.add(e);

		if (i > 0) {  // 0 は NULL 扱い。欠番にする。
			m_objectIndexStack.push(i);
		}
	}

	m_systemAliving = true;
}

void RuntimeManager::dispose()
{
	m_systemAliving = false;
}

LnHandle RuntimeManager::makeObjectWrap(Object* obj)
{
	if (!m_systemAliving) return LN_NULL_HANDLE;
	if (obj == nullptr) return LN_NULL_HANDLE;

	// 登録済みならハンドル (管理配列上のインデックス) を返す
	auto runtimeData = reinterpret_cast<ObjecRuntimeData*>(detail::ObjectHelper::getRuntimeData(obj));
	if (runtimeData)
	{
		RefObjectHelper::retain(obj);
		return runtimeData->index;
	}

	// 管理配列がすべて埋まっている場合
	if (m_objectIndexStack.empty())
	{
		// 末尾に追加する
		ObjectEntry e;
		e.object = obj;
		e.index = m_objectEntryList.size();
		e.externalRefCount = 1;
		m_objectEntryList.add(e);

		// register runtime data
		auto data = LN_NEW ObjecRuntimeData();
		data->index = e.index;
		detail::ObjectHelper::setRuntimeData(obj, reinterpret_cast<intptr_t>(data));

		RefObjectHelper::retain(obj);
		return e.index;
	}
	else
	{
		// 空き場所を取得
		int newPos = m_objectIndexStack.top();
		m_objectIndexStack.pop();

		// 格納
		ObjectEntry& e = m_objectEntryList[newPos];
		e.object = obj;
		e.index = newPos;
		e.externalRefCount = 1;

		// register runtime data
		auto data = LN_NEW ObjecRuntimeData();
		data->index = e.index;
		detail::ObjectHelper::setRuntimeData(obj, reinterpret_cast<intptr_t>(data));

		RefObjectHelper::retain(obj);
		return e.index;
	}
}

void RuntimeManager::releaseObject(LnHandle handle)
{
	if (!m_systemAliving) return;

	int index = static_cast<int>(handle);
	ObjectEntry& e = m_objectEntryList[index];

	if (e.object)
	{
		e.externalRefCount--;
		if (e.externalRefCount <= 0)
		{
			RefObjectHelper::release(e.object);
			m_objectIndexStack.push(index);
		}
	}
}

ObjectEntry* RuntimeManager::getObjectEntry(LnHandle handle)
{
	return &m_objectEntryList[static_cast<int>(handle)];
}

void RuntimeManager::setManagedObjectId(LnHandle handle, int64_t id)
{
	auto runtimeData = reinterpret_cast<ObjecRuntimeData*>(detail::ObjectHelper::getRuntimeData(m_objectEntryList[static_cast<int>(handle)].object));
	runtimeData->managedObjectId = id;
}

int64_t RuntimeManager::getManagedObjectId(LnHandle handle)
{
	auto runtimeData = reinterpret_cast<ObjecRuntimeData*>(detail::ObjectHelper::getRuntimeData(m_objectEntryList[static_cast<int>(handle)].object));
	return runtimeData->managedObjectId;
}

LnResult RuntimeManager::processException(Exception* e)
{
	return LN_ERROR_UNKNOWN;
}

} // namespace detail
} // namespace ln

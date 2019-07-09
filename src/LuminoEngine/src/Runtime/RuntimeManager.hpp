#pragma once
#include <stack>
#include <LuminoEngine/Runtime/Common.hpp>

namespace ln {
namespace detail {

struct ObjecRuntimeData
{
	int index = -1;	// inded of RuntimeManager::m_objectEntryList (LnHandle)
	int64_t managedObjectId = -1;
};

struct ObjectEntry
{
	Object*	object = nullptr;
	int index = -1;	// inded of RuntimeManager::m_objectEntryList (LnHandle)
	int externalRefCount = 0;
};

class RuntimeManager
	: public RefObject
{
public:
	struct Settings
	{
	};

	RuntimeManager();
	virtual ~RuntimeManager();
	void init(const Settings& settings);
	void dispose();

	// create の時は想定通りの動作。externalRefCount=1 の状態で作られる。
	// get の場合も同様に作られる。基本方針は COM と同じく、Get したものは Release で解放するべきだが、
	// ObjectEntry は Native 側の Release を受けて強制的に解放されるため、FlatC API を直接使っているときは Get の Release は不要。
	LnHandle makeObjectWrap(Object* obj);

	void releaseObject(LnHandle handle);

	ObjectEntry* getObjectEntry(LnHandle handle);

	void setManagedObjectId(LnHandle handle, int64_t id);
	int64_t getManagedObjectId(LnHandle handle);

	LnResult processException(Exception* e);



private:
	List<ObjectEntry> m_objectEntryList;
	std::stack<int> m_objectIndexStack;
	bool m_systemAliving;
};

} // namespace detail
} // namespace ln



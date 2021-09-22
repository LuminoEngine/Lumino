#pragma once
#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Engine/Module.hpp>
#include <LuminoFFI/Common.hpp>
#include <LuminoFFI/Runtime.hpp>

namespace ln {
namespace detail {

struct ObjectRuntimeData
{
	int index = -1;	// inded of RuntimeManager::m_objectEntryList (LNHandle)
	int64_t managedObjectId = -1;
	bool fromCreate = false;
};

struct ObjectEntry
{
	Object*	object = nullptr;
	int index = -1;	// inded of RuntimeManager::m_objectEntryList (LNHandle)

	// 1~ である場合、fromCreate は true でなければならない。
	// 0 になった場合、Handle を無効化し、object->release する。
	// fromCreate が false の場合、この値は常に 0 である。（明示的 Release 禁止）
	// Get (fromCreate=fasle && refCount==0) 後、明示的 Retain すると、fromCreate=true, refCount=1 となり object->retain し、Create と同じ扱いになる。
	int refCount = 0;
};

class RuntimeManager
	: public Module
	, public IObjectEventListener
{
public:
	struct Settings
	{
        LNRuntimeFinalizedCallback runtimeFinalizedCallback = nullptr;
        LNReferenceCountTrackerCallback referenceCountTrackerCallback = nullptr;
        LNRuntimeGetTypeInfoIdCallback runtimeGetTypeInfoIdCallback = nullptr;
	};
	static Settings s_globalSettings;

	static RuntimeManager* initialize(const Settings& settings);
	static void terminate();
	static inline RuntimeManager* instance() { return static_cast<RuntimeManager*>(EngineContext2::instance()->runtimeManager); }


	// create の時は想定通りの動作。externalRefCount=1 の状態で作られる。
	// get の場合も同様に作られる。基本方針は COM と同じく、Get したものは Release で解放するべきだが、
	// ObjectEntry は Native 側の Release を受けて強制的に解放されるため、FlatC API を直接使っているときは Get の Release は不要。
	LNHandle makeObjectWrap(Object* obj, bool fromCreate);

	void retainObjectExplicitly(LNHandle handle);
	void releaseObjectExplicitly(LNHandle handle);
	void onDestructObject(Object* obj) override;

	ObjectEntry* getObjectEntry(LNHandle handle);
    Object* getObjectFromHandle(LNHandle handle);

	void setManagedObjectId(LNHandle handle, int64_t id);
	int64_t getManagedObjectId(LNHandle handle);
	int64_t getManagedTypeInfoId(LNHandle handle);
	
	//static void setReferenceCountTracker(LNReferenceCountTrackerCallback callback);
	void setReferenceTrackEnabled(LNHandle handle);
	void onRetainedObject(Object* obj) override;
	void onReleasedObject(Object* obj) override;

    //static void setRuntimeFinalizedCallback(LNRuntimeFinalizedCallback callback);
    //static void setRuntimeCreateInstanceCallback(LNRuntimeCreateInstanceCallback callback);
    //static void setRuntimeGetTypeInfoIdCallback(LNRuntimeGetTypeInfoIdCallback callback);

	LNResult processException(Exception* e);
	void dumpInfo() const;

    RuntimeStringBuffer* requestCommonStringBuffer() { return &m_commonStringBuffer; }

	void setAStringEncoding(TextEncoding* value);
	TextEncoding* getAStringEncoding() const;

private:
	RuntimeManager();
	virtual ~RuntimeManager();
	bool init(const Settings& settings);
	void dispose();

    Settings m_settings;
	List<ObjectEntry> m_objectEntryList;
	std::stack<int> m_objectIndexStack;
	bool m_systemAliving;
	std::mutex m_mutex;
    RuntimeStringBuffer m_commonStringBuffer;
};

} // namespace detail
} // namespace ln



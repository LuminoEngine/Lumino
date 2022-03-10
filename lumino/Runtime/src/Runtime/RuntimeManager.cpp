#include <LuminoEngine/Reflection/Object.hpp>
#include <LuminoEngine/Reflection/TypeInfo.hpp>
#include "../../LuminoEngine/src/Engine/EngineDomain.hpp"
#include <LuminoEngine/Runtime/detail/RuntimeManager.hpp>

namespace ln {
namespace detail {

//==============================================================================
// RuntimeStringBuffer

RuntimeStringBuffer::RuntimeStringBuffer()
    : m_translated(false) {
}

// void RuntimeStringBuffer::init(const String& str)
//{
//     Object::init();
//     m_str = str;

void RuntimeStringBuffer::reset(const String& str) {
    m_str = str;
    m_translated = false;
}

const char* RuntimeStringBuffer::getAscii() {
    if (!m_translated) {
        m_strAscii = m_str.toStdString(); // TODO: encoding
        m_translated = true;
    }
    return m_strAscii.c_str();
}

//==============================================================================
// RuntimeManager

RuntimeManager::Settings RuntimeManager::s_globalSettings;

RuntimeManager* RuntimeManager::initialize(const Settings& settings)
{
	if (instance()) return instance();

	auto m = Ref<RuntimeManager>(LN_NEW detail::RuntimeManager(), false);
	if (!m->init(settings)) return nullptr;

	EngineContext2::instance()->registerModule(m);
	EngineContext2::instance()->runtimeManager = m;
	return m;
}

void RuntimeManager::terminate()
{
	if (instance()) {
		instance()->dispose();
		EngineContext2::instance()->unregisterModule(instance());
		EngineContext2::instance()->runtimeManager = nullptr;
	}
}


//LNReferenceCountTrackerCallback RuntimeManager::m_referenceCountTracker = nullptr;
//LNRuntimeFinalizedCallback RuntimeManager::m_runtimeFinalizedCallback = nullptr;

RuntimeManager::RuntimeManager()
	: m_systemAliving(false)
{
//    // 特に Binding-module にて、Engine 初期化以前にいろいろ処理を行うが、
//    // そこのログを出力したいので他の設定より先に有効化しておく。
//    ln::Logger::addStdErrAdapter();
//#ifdef LN_OS_ANDROID
//    ln::Logger::addLogcatAdapter();
//#endif
//#if defined(LN_OS_MAC) || defined(LN_OS_IOS)
//    ln::Logger::addNLogAdapter();
//#endif
}

RuntimeManager::~RuntimeManager()
{
}

bool RuntimeManager::init(const Settings& settings)
{
	LN_LOG_DEBUG("RuntimeManager Initialization started.");

    m_settings = settings;

	setAStringEncoding(TextEncoding::utf8Encoding());

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

	//EngineContext2::instance()->m_onRetainedObject = [](EngineContext2* context, Object* obj) {
	//	static_cast<RuntimeManager*>(context->runtimeManager)->onRetainedObject(obj);
	//};
	//EngineContext2::instance()->m_onReleasedObject = [](EngineContext2* context, Object* obj) {
	//	static_cast<RuntimeManager*>(context->runtimeManager)->onReleasedObject(obj);
	//};
	EngineContext2::initialize();
	assert(!EngineContext2::instance()->objectEventListener);
	EngineContext2::instance()->objectEventListener = this;

	LN_LOG_DEBUG("RuntimeManager Initialization finished.");
	return true;
}

void RuntimeManager::dispose()
{
	LN_LOG_DEBUG("RuntimeManager finalization started.");

	EngineContext2::instance()->objectEventListener = nullptr;

	for (auto& e : m_objectEntryList) {
		if (e.object) {
			auto runtimeData = detail::ObjectHelper::getRuntimeData(e.object);
			if (runtimeData->fromCreate) {
				RefObjectHelper::release(e.object);	// If the reference count reaches 0, onDestructObject is called.
			}
		}
	}

	m_systemAliving = false;
    m_settings.referenceCountTrackerCallback = nullptr;

    if (m_settings.runtimeFinalizedCallback) {
        m_settings.runtimeFinalizedCallback();
        m_settings.runtimeFinalizedCallback = nullptr;
    }

	LN_LOG_DEBUG("RuntimeManager finalization finished.");
}

LNHandle RuntimeManager::makeObjectWrap(Object* obj, bool fromCreate)
{
	if (!m_systemAliving) return LN_NULL_HANDLE;
	if (obj == nullptr) return LN_NULL_HANDLE;

	if (fromCreate) {
		RefObjectHelper::retain(obj);
	}

	// 登録済みならハンドル (管理配列上のインデックス) を返す
	auto runtimeData = detail::ObjectHelper::getRuntimeData(obj);
	if (runtimeData)
	{
		return runtimeData->index;
	}

	// 管理配列がすべて埋まっている場合
	if (m_objectIndexStack.empty())
	{
		// 末尾に追加する
		ObjectEntry e;
		e.object = obj;
		e.index = m_objectEntryList.size();
		e.refCount = (fromCreate) ? 1 : 0;
		m_objectEntryList.add(e);

		// register runtime data
		auto data = LN_NEW ObjectRuntimeData();
		data->index = e.index;
		data->fromCreate = fromCreate;
		detail::ObjectHelper::setRuntimeData(obj, data);

		LN_LOG_DEBUG("nid registerd: {}", e.index);
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
		e.refCount = (fromCreate) ? 1 : 0;

		// register runtime data
		auto data = LN_NEW ObjectRuntimeData();
		data->index = e.index;
		data->fromCreate = fromCreate;
		detail::ObjectHelper::setRuntimeData(obj, data);

		LN_LOG_DEBUG("nid registerd: {}", e.index);
		return e.index;
	}
}

void RuntimeManager::retainObjectExplicitly(LNHandle handle)
{
	auto& e = m_objectEntryList[static_cast<int>(handle)];
	RefObjectHelper::retain(e.object);
#if 0
	auto& e = m_objectEntryList[static_cast<int>(handle)];
	auto* runtimeData = detail::ObjectHelper::getRuntimeData(e.object);
	
	if (!runtimeData->fromCreate && e.refCount == 0) {
		// generated by "Get", retain native object.
		runtimeData->fromCreate = true;
		e.refCount = 1;
		RefObjectHelper::retain(e.object);
	}
	else if (runtimeData->fromCreate && e.refCount > 0) {
		// generated by "Create"
		e.refCount++;
	}
	else {
		LN_UNREACHABLE();
	}
#endif
}

void RuntimeManager::releaseObjectExplicitly(LNHandle handle)
{
	if (!m_systemAliving) return;

	int index = static_cast<int>(handle);
	ObjectEntry& e = m_objectEntryList[index];


	// 明示的に release したので自動 release は不要
	auto runtimeData = detail::ObjectHelper::getRuntimeData(e.object);
	runtimeData->fromCreate = false;

	RefObjectHelper::release(e.object);		// If the reference count reaches 0, onDestructObject is called.
}

void RuntimeManager::onDestructObject(Object* obj)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto runtimeData = detail::ObjectHelper::getRuntimeData(obj);
	if (runtimeData) {
		ObjectEntry& e = m_objectEntryList[runtimeData->index];
		LN_CHECK(e.object == obj);

		e.refCount--;
		if (e.refCount <= 0) {
			m_objectIndexStack.push(runtimeData->index);
			e.object = nullptr;
		}
		LN_LOG_DEBUG("nid unregisterd: {}", e.index);

		delete runtimeData;
	}
}

ObjectEntry* RuntimeManager::getObjectEntry(LNHandle handle)
{
	return &m_objectEntryList[static_cast<int>(handle)];
}

Object* RuntimeManager::getObjectFromHandle(LNHandle handle)
{
    return m_objectEntryList[static_cast<int>(handle)].object;
}

void RuntimeManager::setManagedObjectId(LNHandle handle, int64_t id)
{
	auto runtimeData = detail::ObjectHelper::getRuntimeData(m_objectEntryList[static_cast<int>(handle)].object);
	runtimeData->managedObjectId = id;
}

int64_t RuntimeManager::getManagedObjectId(LNHandle handle)
{
	auto runtimeData = detail::ObjectHelper::getRuntimeData(m_objectEntryList[static_cast<int>(handle)].object);
	return runtimeData->managedObjectId;
}

int64_t RuntimeManager::getManagedTypeInfoId(LNHandle handle)
{
	auto obj = m_objectEntryList[static_cast<int>(handle)].object;
	auto typeInfo = TypeInfo::getTypeInfo(obj);
	return detail::TypeInfoInternal::getManagedTypeInfoId(typeInfo);
}

//void RuntimeManager::setReferenceCountTracker(LNReferenceCountTrackerCallback callback)
//{
//	m_referenceCountTracker = callback;
//}
//
//void RuntimeManager::setRuntimeFinalizedCallback(LNRuntimeFinalizedCallback callback)
//{
//    m_runtimeFinalizedCallback = callback;
//}

//void RuntimeManager::setRuntimeCreateInstanceCallback(LNRuntimeCreateInstanceCallback callback)
//{
//    LN_NOTIMPLEMENTED();
//}
//
//void RuntimeManager::setRuntimeGetTypeInfoIdCallback(LNRuntimeGetTypeInfoIdCallback callback)
//{
//    LN_NOTIMPLEMENTED();
//}

void RuntimeManager::setReferenceTrackEnabled(LNHandle handle)
{
	RefObjectInternal::setObjectFlag(m_objectEntryList[static_cast<int>(handle)].object, RefObjectFlags_ReferenceTracking, true);
}

void RuntimeManager::onRetainedObject(Object* obj)
{
	if (m_settings.referenceCountTrackerCallback) {
		if (auto runtimeData = detail::ObjectHelper::getRuntimeData(obj)) {
            m_settings.referenceCountTrackerCallback(runtimeData->index, LNI_REFERENCE_RETAINED, RefObjectHelper::getReferenceCount(obj));
		}
	}
}

void RuntimeManager::onReleasedObject(Object* obj)
{
	if (m_settings.referenceCountTrackerCallback) {
		if (auto runtimeData = detail::ObjectHelper::getRuntimeData(obj)) {
            m_settings.referenceCountTrackerCallback(runtimeData->index, LNI_REFERENCE_RELEASED, RefObjectHelper::getReferenceCount(obj));
		}
	}
}

void RuntimeManager::dumpInfo() const
{
	std::cout << std::endl;
	std::cout << "Native alive objects" << std::endl;
	std::cout << "----------" << std::endl;

	for (auto& entry : m_objectEntryList) {
		if (entry.object) {
			std::cout << entry.index << ": ";
			std::cout << entry.object;
			std::cout << "(" << TypeInfo::getTypeInfo(entry.object)->name() << ")";
			std::cout << ", refc: " << RefObjectHelper::getReferenceCount(entry.object) << std::endl;
		}
	}
	std::cout << "----------" << std::endl;
	std::cout << std::endl;
}

void RuntimeManager::setAStringEncoding(TextEncoding* value)
{
	m_commonStringBuffer.setAStringEncoding(value);
}

TextEncoding* RuntimeManager::getAStringEncoding() const
{
	return m_commonStringBuffer.getAStringEncoding();
}

} // namespace detail
} // namespace ln

/**
	@file	Cache.h
*/
#pragma once

#include <map>
#include "String.h"
#include "RefObject.h"
#include "../IO/PathName.h"
#include "../Threading/Mutex.h"

LN_NAMESPACE_BEGIN
class CacheManager;

/**
	@brief	キャッシュの検索キーです。
*/
class CacheKey
{
public:
	static const CacheKey Null;

public:
	CacheKey();
	explicit CacheKey(uint64_t hashCode);
	explicit CacheKey(const TCHAR* str);
	explicit CacheKey(const String& str);
	explicit CacheKey(const StringRef& str);
	explicit CacheKey(const PathName& path);

	bool operator == (const CacheKey& key) const;
	bool operator < (const CacheKey& key) const;

	bool IsNull() const;

private:

	enum Type
	{
		Type_Null = 0,
		Type_HashCode,
		Type_String,
		Type_PathName,
	};

	Type		m_keyType;
	uint64_t	m_hashCode;
	String		m_string;
	PathName	m_pathName;
};

/**
	@brief		キャッシュ可能なオブジェクトのインターフェイスです。
	@details	ICacheObject は RefObject とセットで使用します。
				両方を継承したクラスに LN_CACHE_OBJECT_DECL マクロで必要な機能を実装します。
				ICacheObject は参照カウントが 0 になったとき、delete はされずキャッシュに入れられます。
*/
class ICacheObject
{
protected:
	friend class CacheManager;
	struct CacheObjectInfo
	{
		CacheManager*	manager;
		CacheKey		key;
		size_t			cacheMemorySize;
		bool			inCacheList;

		CacheObjectInfo();
		~CacheObjectInfo();
	};

public:
	ICacheObject() {}

protected:
	virtual ~ICacheObject() {}
	virtual CacheObjectInfo&	GetCacheObjectInfo() = 0;

public:
	virtual int32_t	AddRef() = 0;
	virtual int32_t	Release() = 0;
};

/// ICacheObject の実装ユーティリティ
#define LN_CACHE_OBJECT_DECL \
protected: \
	CacheObjectInfo m_cacheObjectInfo; \
public: \
	virtual CacheObjectInfo& GetCacheObjectInfo() { return m_cacheObjectInfo; } \
	virtual int32_t AddRef() { return RefObject::AddRef(); } \
	virtual int32_t Release() \
	{ \
		if (m_cacheObjectInfo.manager == NULL/* || m_cacheObjectInfo.mIsStockObject*/) \
		{ \
			return RefObject::Release(); \
		} \
		int32_t count = m_referenceCount.Decrement(); \
		LN_ASSERT(count >= 0); \
		if (count == 0) \
		{ \
			m_cacheObjectInfo.manager->AddCacheUnusedList(this); \
		} \
		return count; \
	}

/**
	@brief	キャッシュオブジェクトの管理クラスです。
*/
class CacheManager
	: public RefObject
{
public:
	/**
		@brief		初期化
		@param[in]	capacityCount	: キャッシュに保持できる最大オブジェクト数
		@param[in]	maxMemorySize	: キャッシュに保持できる最大メモリ量 (byte単位。0 の場合はメモリ量を考慮しない)
	*/
	CacheManager(int capacityCount, size_t maxMemorySize);

public:

	/**
		@brief	終了処理 (これ以降キャッシュにオブジェクトを追加しないようにする)
	*/
	void FinalizeCache();

	/**
		@brief	オブジェクトをキャッシュ管理に追加します。
	*/
	void RegisterCacheObject(const CacheKey& key, ICacheObject* obj);

	/**
		@brief		キーに一致するオブジェクトを検索します。
		@details	参照カウントをインクリメントして返します。
	*/
	ICacheObject* FindObjectAddRef(const CacheKey& key);
	
	/**
		@brief	キャッシュをクリアします。
	*/
	void ClearCache();

public:
	void AddCacheUnusedList(ICacheObject* obj);
	void DeleteCachedObject(ICacheObject* obj);

private:
	virtual ~CacheManager();

private:
	friend class ICacheObject;
	class CacheUnusedList;
	typedef std::map<CacheKey, ICacheObject*>	CacheUsingMap;
	typedef std::pair<CacheKey, ICacheObject*>	CacheUsingPair;

	CacheUnusedList*	m_cacheUnusedList;
	CacheUsingMap		m_cacheUsingMap;
	Mutex				m_mutex;
};

LN_NAMESPACE_END

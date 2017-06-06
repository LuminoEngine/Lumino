
#include <Lumino/Base/Common.h>
#include <Lumino/Base/Stack.h>
#include <Lumino/Base/Cache.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// CacheKey
//==============================================================================

const CacheKey CacheKey::Null;

//------------------------------------------------------------------------------
CacheKey::CacheKey()
	: m_keyType(Type_Null)
	, m_hashCode(0)
	, m_string()
	, m_pathName()
{
}

//------------------------------------------------------------------------------
CacheKey::CacheKey(uint64_t hashCode)
	: m_keyType(Type_HashCode)
	, m_hashCode(hashCode)
	, m_string()
	, m_pathName()
{
}

//------------------------------------------------------------------------------
CacheKey::CacheKey(const TCHAR* str)
	: m_keyType(Type_String)
	, m_hashCode(0)
	, m_string(str)
	, m_pathName()
{
}

//------------------------------------------------------------------------------
CacheKey::CacheKey(const String& str)
	: m_keyType(Type_String)
	, m_hashCode(0)
	, m_string(str)
	, m_pathName()
{
}

//------------------------------------------------------------------------------
CacheKey::CacheKey(const StringRef& str)
	: m_keyType(Type_String)
	, m_hashCode(0)
	, m_string(str)
	, m_pathName()
{
}

//------------------------------------------------------------------------------
CacheKey::CacheKey(const PathName& path)
	: m_keyType(Type_PathName)
	, m_hashCode(0)
	, m_string()
	, m_pathName(path)
{
}

//------------------------------------------------------------------------------
bool CacheKey::operator == (const CacheKey& key) const
{
	if (m_keyType != key.m_keyType) {
		return false;
	}

	switch (m_keyType)
	{
	case Type_Null:
		return true;
	case Type_HashCode:
		return (m_hashCode == key.m_hashCode);
	case Type_String:
		return (m_string == key.m_string);
	case Type_PathName:
		return (m_pathName == key.m_pathName);
	}

	LN_ASSERT(0);	// ここに来るのは NG
	return false;
}

//------------------------------------------------------------------------------
bool CacheKey::operator < (const CacheKey& key) const
{
	if (m_keyType != key.m_keyType) {
		return m_keyType < key.m_keyType;
	}

	switch (m_keyType)
	{
	case Type_Null:
		return false;
	case Type_HashCode:
		return (m_hashCode < key.m_hashCode);
	case Type_String:
		return (m_string < key.m_string);
	case Type_PathName:
		return (m_pathName < key.m_pathName);
	}

	LN_ASSERT(0);	// ここに来るのは NG
	return false;
	// map,set が検索を行う時は「X1 < X2」「X2 < X1」の両方が成立しなかったとき「X1==X2」とみなすのが標準的な動作らしい
}


//------------------------------------------------------------------------------
bool CacheKey::IsNull() const
{
	return (m_keyType == Type_Null);
}

//==============================================================================
// CacheManager::CacheUnusedList
//==============================================================================
class CacheManager::CacheUnusedList
{
public:
	CacheUnusedList(CacheManager* manager)
		: m_manager(manager)
		, mNodes(NULL)
		, m_internalCapacity(0)
		, mSize(0)
		, m_usedMemorySize(0)
		, m_maxMemorySize(ULLONG_MAX)
	{}

	~CacheUnusedList()
	{
		Clear();
		LN_SAFE_DELETE_ARRAY(mNodes);
	}

public:

	/// 最大要素数の取得
	int GetCapacity() const { return m_internalCapacity - 1; }

	/// 領域の確保
	void SetCapacity(int capacityCount, size_t maxMemorySize)
	{
		Clear();
		LN_SAFE_DELETE_ARRAY(mNodes);

		if (capacityCount == 0)
		{
			m_internalCapacity = 0;
			return;
		}
		m_maxMemorySize = maxMemorySize;
		if (m_maxMemorySize == 0) {
			m_maxMemorySize = ULLONG_MAX;
		}

		// リストのオブジェクト解放は、新しいオブジェクトが追加された後で行う。
		// そのため、リストの要素数は +1 多く保持できるようにする必要がある。
		m_internalCapacity = capacityCount + 1;
		mNodes = LN_NEW Node[m_internalCapacity + 1];	// ダミーノードの分の +1 (mNodes[m_internalCapacity] がダミーノード)

		// 空き番号スタック
		mIndexStack.Reserve(m_internalCapacity);
		for (int i = 0; i < m_internalCapacity; ++i)
		{
			mIndexStack.Push(i);
		}

		// m_internalCapacity 番は最初の要素の前かつ最後の要素の後にあるダミー要素
		mNodes[m_internalCapacity].next = mNodes[m_internalCapacity].prev = m_internalCapacity;
	}

	/// クリア (全 delete 呼び出し)
	void Clear()
	{
		if (mNodes && m_internalCapacity > 0)
		{
			Node* node = &mNodes[m_internalCapacity];	// ダミーノード
			uint32_t index = node->next;             // ダミーノードの次が開始インデックス
			int i = 0;

			while (true)
			{
				// ダミーノードを指している場合は終了
				if (index == m_internalCapacity)
				{
					break;
				}
				// 削除
				m_manager->DeleteCachedObject(mNodes[index].value);
				// 次へ
				index = mNodes[index].next;

				++i;
			}

			mNodeMap.clear();

			// 空き番号スタック
			mIndexStack.Clear();
			for (int i = 0; i < m_internalCapacity; ++i)
			{
				mIndexStack.Push(i);
			}

			// ノードは new とかしてるわけじゃないので、ダミーの前後だけ付け変えておけばリストは空の状態になる
			mNodes[m_internalCapacity].next = mNodes[m_internalCapacity].prev = m_internalCapacity;
			mSize = 0;
			m_usedMemorySize = 0;
		}
	}

	/// オブジェクトを追加する (メモリサイズの都合などで追加できなかった場合は false)
	bool AddObject(ICacheObject* obj)
	{
		ICacheObject::CacheObjectInfo& info = obj->GetCacheObjectInfo();

		if (obj && !info.key.IsNull() && m_internalCapacity > 0)
		{
			// すでに追加されているか調べる
			if (info.inCacheList) {
				return false;
			}
			// オブジェクト単体のサイズがキャッシュメモリ量を超えている。追加できない
			if (info.cacheMemorySize > m_maxMemorySize) {
				return false;
			}

			int idx = mIndexStack.GetTop();
			mIndexStack.Pop();

			Node* dummy = &mNodes[m_internalCapacity];	    // ダミーノード
			Node* prev = &mNodes[dummy->prev];	// ダミーのひとつ前 ( 最後 )
			Node* new_node = &mNodes[idx];           // その間に入れるノード

			// new_node をリストの末尾に追加する
			new_node->prev = dummy->prev;
			new_node->next = m_internalCapacity;
			dummy->prev = idx;
			prev->next = idx;

			new_node->value = obj;

			mNodeMap.insert(std::pair< CacheKey, Node* >(info.key, new_node));

			// 削除待ち中を示すフラグ ON
			info.inCacheList = true;

			// キャッシュ内メモリサイズを増やす
			m_usedMemorySize += info.cacheMemorySize;

			++mSize;

			// キャッシュを調べて古いものがあれば削除
			CheckList();

			return true;
		}
		return false;
	}

	/// キーに一致するオブジェクトの検索 (返されるオブジェクトの参照カウントは常に 0)             
	ICacheObject* FindObject(const CacheKey& key)
	{
		if (m_internalCapacity == 0) return NULL;

		NodeMap::iterator itr = mNodeMap.find(key);
		if (itr != mNodeMap.end())
		{
			// 削除待ちリストから外す
			Node* n = itr->second;
			uint32_t idx = mNodes[n->next].prev;   // 自分のインデックス
			mNodes[n->next].prev = n->prev;
			mNodes[n->prev].next = n->next;

			// 空きインデックスを戻す
			mIndexStack.Push(idx);

			ICacheObject* v = itr->second->value;

			mNodeMap.erase(itr);

			--mSize;

			// 使用中にする
			v->GetCacheObjectInfo().inCacheList = false;

			return v;
		}
		return NULL;
	}

	/// キャッシュの内容を出力する
	void dump(FILE* stream);

private:

	/// キャッシュのリストを調べて、古いもの(先頭)がある場合は削除
	void CheckList()
	{
		// オブジェクト数とメモリサイズ両方が制限内に収まるまで削除を繰り返す
		while (mSize >= m_internalCapacity || m_usedMemorySize > m_maxMemorySize)
		{
			Node* dummy = &mNodes[m_internalCapacity];	    // ダミーノード
			uint32_t front_index = dummy->next;
			Node* front = &mNodes[front_index];	// 先頭のノード取り出し

			// front をリストから外す
			dummy->next = front->next;
			mNodes[front->next].prev = m_internalCapacity;

			// 空いたインデックスをスタックに積む
			mIndexStack.Push(front_index);

			// Map からも削除
			mNodeMap.erase(front->value->GetCacheObjectInfo().key);

			--mSize;

			// キャッシュ内メモリサイズを減らす
			m_usedMemorySize -= front->value->GetCacheObjectInfo().cacheMemorySize;

			// ないと思うけど、一応 front がダミーじゃないかチェックしてから解放
			if (front != dummy)
			{
				m_manager->DeleteCachedObject(front->value);
			}
		}
	}

private:

	struct Node
	{
		int				next;
		int				prev;
		ICacheObject*	value;
	};

	typedef std::map<CacheKey, Node*>    NodeMap;

private:
	CacheManager*	m_manager;
	Node*           mNodes;         ///< 解放待ちリスト
	int             m_internalCapacity;      ///< 最大要素数 (SetCapacity で指定されたサイズ +1)
	int             mSize;          ///< 現在の要素数
	Stack<int>		mIndexStack;    ///< 空き番号スタック
	NodeMap         mNodeMap;       ///< キーとノードの対応表
	uint64_t		m_usedMemorySize;
	uint64_t		m_maxMemorySize;
};

//==============================================================================
// ICacheObject::CacheObjectInfo
//==============================================================================

//------------------------------------------------------------------------------
ICacheObject::CacheObjectInfo::CacheObjectInfo()
	: manager(NULL)
	, key()
	, cacheMemorySize(0)
	, inCacheList(false)
	//, IsStockObject(false)
{
}

//------------------------------------------------------------------------------
ICacheObject::CacheObjectInfo::~CacheObjectInfo()
{
	LN_SAFE_RELEASE(manager);
}


//==============================================================================
// CacheManager
//==============================================================================

//------------------------------------------------------------------------------
CacheManager::CacheManager(int capacityCount, size_t maxMemorySize)
	: m_cacheUnusedList(NULL)
{
	m_cacheUnusedList = LN_NEW CacheUnusedList(this);
	m_cacheUnusedList->SetCapacity(capacityCount, maxMemorySize);
}

//------------------------------------------------------------------------------
CacheManager::~CacheManager()
{
	FinalizeCache();
}

//------------------------------------------------------------------------------
void CacheManager::FinalizeCache()
{
	ClearCache();
	LN_SAFE_DELETE(m_cacheUnusedList);
}

//------------------------------------------------------------------------------
void CacheManager::RegisterCacheObject(const CacheKey& key, ICacheObject* obj)
{
	MutexScopedLock lock(m_mutex);

	ICacheObject::CacheObjectInfo& info = obj->GetCacheObjectInfo();

	LN_ASSERT(!key.IsNull());					// キーは中身が無ければならない
	LN_ASSERT(info.key.IsNull());		// 既にキーが割り当てられている obj は登録済みなのでエラー
	if (m_cacheUnusedList == NULL) { return; }	// 初期化されていなければ何もしない

	m_cacheUsingMap.insert(CacheUsingPair(key, obj));
	info.manager = this;
	info.manager->AddRef();
	info.key = key;
}

//------------------------------------------------------------------------------
ICacheObject* CacheManager::FindObjectAddRef(const CacheKey& key)
{
	MutexScopedLock lock(m_mutex);

	ICacheObject* obj = NULL;
	if (!key.IsNull() && m_cacheUnusedList != NULL)
	{
		// まずは今使ってる物を検索
		CacheUsingMap::iterator itr = m_cacheUsingMap.find(key);
		if (itr != m_cacheUsingMap.end())
		{
			obj = itr->second;
			obj->AddRef();
		}
		// キャッシュリストを検索
		else
		{
			obj = m_cacheUnusedList->FindObject(key);
			if (obj != NULL)
			{
				// 使用中オブジェクトマップに入れておく
				m_cacheUsingMap.insert(CacheUsingPair(obj->GetCacheObjectInfo().key, obj));
				obj->AddRef();
			}
		}
	}
	return obj;
}

//------------------------------------------------------------------------------
void CacheManager::ClearCache()
{
	MutexScopedLock lock(m_mutex);
	if (m_cacheUnusedList != NULL) {	// Finalize() 済みチェック
		m_cacheUnusedList->Clear();
	}
}

//------------------------------------------------------------------------------
void CacheManager::AddCacheUnusedList(ICacheObject* obj)
{
	LN_ASSERT(obj != NULL);
	bool addedObj = false;
	{
		/*	この関数の最後で DeleteCachedObject を呼び出すが、
			obj がこの CacheManager を参照する最後のオブジェクトである場合
			delete したとき、CacheManager も delete される。
			その状態で MutexScopedLock が Unlock すると解放済みのオブジェクトを操作することになるので NG。
			DeleteCachedObject() は排他処理したくないので、ブロックをひとつ下げて逃げる。
		*/
		MutexScopedLock lock(m_mutex);

		if (m_cacheUnusedList != NULL)	// Finalize() 済みチェック
		{
			m_cacheUsingMap.erase(obj->GetCacheObjectInfo().key);
			addedObj = m_cacheUnusedList->AddObject(obj);
		}
	}

	// 追加できなかったら、参照カウント 0 なのにキャッシュに入れなかったということ。delete する
	if (!addedObj) {
		DeleteCachedObject(obj);
	}
}

//------------------------------------------------------------------------------
void CacheManager::DeleteCachedObject(ICacheObject* obj)
{
	// この関数は AddCacheUnusedList() => m_cacheUnusedList->AddObject 経由で呼ばれるので、
	// ロックするような仕様が必要になったときは注意すること。

	LN_ASSERT(obj != NULL);
	LN_ASSERT(obj->GetCacheObjectInfo().manager != NULL);

	delete obj;
}

LN_NAMESPACE_END


#pragma once
#include <vector>
#include <Lumino/Base/RefObject.h>

LN_NAMESPACE_BEGIN

/**
	@brief		自動 Release するオブジェクトを管理するプール。
*/
class AutoReleasePool
{
public:

	/**
		@brief		カレントの AutoRseleasePool を取得します。
	*/
	static AutoReleasePool* GetCurrent();

	/**
		@brief		この AutoRseleasePool にオブジェクトを追加します。
	*/
	void AddObject(RefObject* obj);

	/**
		@brief		追加されているオブジェクトの Release() を呼び出し、プールをクリアします。
	*/
	void Clear();

	/**
		@brief		カレントの AutoRseleasePool にオブジェクトを追加するユーティリティです。
	*/
	static void AddObjectToCurrent(RefObject* obj);

private:
	AutoReleasePool();
	~AutoReleasePool();

	std::vector<RefObject*>	m_objectList;
};

LN_NAMESPACE_END

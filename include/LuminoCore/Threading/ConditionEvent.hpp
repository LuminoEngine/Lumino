
#pragma once
#include "Common.hpp"

namespace ln {

/** */
class LN_API ConditionEvent
{
public:
	/** デフォルトコンストラクタ (初期値は locked:false) */
	ConditionEvent();

	/** コンストラクタ (初期値を指定する) */
	explicit ConditionEvent(bool locked);

	/** デストラクタ */
	~ConditionEvent();

public:

    /**  */
	void lock();

	/**  */
	void unlock();

	/**  */
	bool isLocked() const;

	/** unlock 状態になるまで待機する */
    void wait();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(ConditionEvent);
	class Impl;
	Impl* m_impl;
};

} // namespace ln

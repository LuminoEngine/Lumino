
#pragma once
#include "../Base/Common.h"

LN_NAMESPACE_BEGIN
namespace detail { class ConditionFlagImpl; }

/**
	@brief		待ち合わせ機能を持つスレッドセーフな bool 型変数のクラス
*/
class LUMINO_EXPORT ConditionFlag
{
public:
	/** デフォルトコンストラクタ (初期値は false) */
	ConditionFlag();

	/** コンストラクタ (初期値を指定する) */
	explicit ConditionFlag(bool initFlag);

	/** デストラクタ */
	~ConditionFlag();

public:

    /** 値を true にする */
	void SetTrue();

	/** 値を false にする */
	void SetFalse();

	/** 値が true かを判定する */
	bool IsTrue() const;

	/** 値が false かを判定する */
	bool IsFalse() const;

	/** 値が true になるまで待機する */
    void Wait();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(ConditionFlag);
	detail::ConditionFlagImpl*	m_impl;
};

LN_NAMESPACE_END

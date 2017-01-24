
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
class BodyBase;
namespace detail { class PhysicsWorld; }

/// 接触点情報
struct ContactInfo
{
	Vector3		Normal;			///< 接触点法線
	Vector3		Position;		///< 接触点位置
	BodyBase*	TargetBody;		///< 接触ボディ
};

/// 各種 Body のベースクラス
class BodyBase
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	typedef List<ContactInfo>	ContactInfoList;

	/// 接触点情報のリスト
	const ContactInfoList& GetContactInfoList() const { return m_contactList; }

	/// ユーザーデータの設定
	void SetUserData(void* data) { m_userData = data; }

	/// ユーザーデータの取得
	void* GetUserData() const { return m_userData; }

LN_INTERNAL_ACCESS:
	void ClearContactList() { m_contactList.Clear(); }
	void AddContact(const ContactInfo& contact) { m_contactList.Add(contact); }

protected:
	BodyBase();
	virtual ~BodyBase();
	void Initialize(btCollisionObject* obj);

private:
	void*					m_userData;
	ContactInfoList			m_contactList;
};

LN_NAMESPACE_END

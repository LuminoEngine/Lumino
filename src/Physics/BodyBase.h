
#pragma once

#include "Common.h"

namespace Lumino
{
namespace Physics
{

/// 接触点情報
struct ContactInfo
{
	Vector3		Normal;			///< 接触点法線
	Vector3		Position;		///< 接触点位置
	BodyBase*	TargetBody;		///< 接触ボディ
};

/// 各種 Body のベースクラス
class BodyBase
    : public RefObject
{
public:
	typedef ArrayList<ContactInfo>	ContactInfoList;

public:
    BodyBase();
    virtual ~BodyBase();

public:

	/// 初期化
	void Create(PhysicsManager* manager, btCollisionObject* obj);

	/// 種類
	virtual BodyType GetBodyType() const = 0;

	/// 接触点情報のリスト
	const ContactInfoList& GetContactInfoList() const { return m_contactList; }

	/// ユーザーデータの設定
	void SetUserData(void* data) { m_userData = data; }

	/// ユーザーデータの取得
	void* GetUserData() const { return m_userData; }

public:	// internal
	///// シミュレーション更新直前の処理
	//virtual void preUpdate() {}

	///// シミュレーション更新直後の処理
	//virtual void postUpdate() {}

	/// デバッグ用の形状描画
	virtual void DrawDebugShape(IDebugRenderer* renderer) {}

	void ClearContactList() { m_contactList.Clear(); }

	void AddContact(const ContactInfo& contact) { m_contactList.Add(contact); }

protected:
	PhysicsManager*		m_manager;
	void*				m_userData;
	ContactInfoList		m_contactList;
};

} // namespace Physics
} // namespace Lumino


#pragma once
#include "Common.h"
#include "IdentifierMap.h"

LN_NAMESPACE_BEGIN
namespace parser
{

class Reference;


enum class EntityKind
{
	None = 0x0000,
	File = 0x0001,
	Macro = 0x0002,	/**< プリプロセッサマクロ */
};

/**
	@brief	Kind は Entity または Reference の検索キー1つ分を表す。または AND の検索キー
*/
class Kind
	//: public RefObject
{
public:
	EntityKind	kindSummary = EntityKind::None;
	//Kind();
	//virtual ~Kind();

//private:
//	LN_DISALLOW_COPY_AND_ASSIGN(Kind);
//	KindSummary	m_kindSummary;
};

/**
	@brief
*/
class KindList
	: public RefObject
{
public:
	KindList();
	virtual ~KindList();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(KindList);
	Array<Kind>	m_kinds;	// OR の検索キー
};
	
/**
	@brief
	@note	Entity は大量にインスタンスが作られる。追加のデータが必要なものは派生させ、メモリ使用量は必要最低限にしたい。
*/
class Entity
	: public RefObject
{
public:
	Entity();
	virtual ~Entity();

	Kind	kind;

	Array<Reference*>	referenceList;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Entity);
};
typedef RefPtr<Entity>	EntityPtr;

/**
	@brief
*/
class FileEntity
	: public Entity
{
public:
	FileEntity();
	virtual ~FileEntity();

	TokenPathName	filePath;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(FileEntity);
};
typedef RefPtr<Entity>	EntityPtr;

/**
	@brief	
	@note	検索も使用したいのでビットフラグとしている。
*/
LN_ENUM_FLAGS(ReferenceKindFlags)
{
	None = 0x0000,

	/**
		マクロ定義も。
	*/
	Define = 0x0001,

	Use = 0x0002,

	UseBy = 0x0004,
};
LN_ENUM_FLAGS_DECLARE(ReferenceKindFlags);

/**
	@brief	
*/
class ReferenceKind
{
public:
	ReferenceKindFlags	kind = ReferenceKindFlags::None;
};

/**
	@brief
*/
class Reference
	: public RefObject
{
public:
	Reference();
	virtual ~Reference();

	ReferenceKind	kind;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Reference);
};
typedef RefPtr<Reference>	ReferencePtr;

/**
	@brief
*/
class EntityDatabase
	: public RefObject
{
public:
	EntityDatabase();
	virtual ~EntityDatabase();

	Array<EntityPtr>	allEntities;	/** 全 Entity */
	Array<ReferencePtr>	allReferences;	/** 全 Reference */

	Entity* CreateEntity(const TokenStringRef& fullIdentifierName);
	Entity* FindEntity(const TokenStringRef& fullIdentifierName);

	Reference* CreateReference(Entity* ownerEntity, ReferenceKindFlags kind);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(EntityDatabase);
	IdentifierMap<Entity*>	m_entityMap;

};

/**
	@brief
*/
class ReferenceTracker
{
public:
	ReferenceTracker(){}
	~ReferenceTracker(){}

	void Initialize(EntityDatabase* ownerDatabase);

	void InjectReference(Scope* baseScope, const TokenStringRef& identiferName, EntityKind entityKind, ReferenceKindFlags referenceKindFlags, Entity* file = nullptr, int line = -1, int column = -1);

private:
	LN_DISALLOW_COPY_AND_ASSIGN(ReferenceTracker);
	EntityDatabase* m_ownerDatabase;
};

} // namespace Parser
LN_NAMESPACE_END

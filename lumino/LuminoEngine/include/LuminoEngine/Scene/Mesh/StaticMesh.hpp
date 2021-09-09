
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class MeshModel;
class MeshImportSettings;

/**
 * StaticMesh
 */
LN_CLASS()
class StaticMesh
	: public VisualObject
{
    LN_OBJECT;
	LN_BUILDER;
public:
	/** load */
	LN_METHOD()
	static Ref<StaticMesh> load(const StringRef& filePath, MeshImportSettings* settings = nullptr);

    static Ref<StaticMesh> create();
    static Ref<StaticMesh> create(MeshModel* model);
    
	/**指定した名前の MeshContainer から、衝突判定用の Body を作成します。 */
	LN_METHOD(Property)
	MeshModel* model() const;

	/**指定した名前の MeshContainer から、衝突判定用の Body を作成します。 */
	LN_METHOD()
	void makeCollisionBody(StringRef meshContainerName);

    MeshComponent* staticMeshComponent() const;

protected:
    void serialize(Serializer2& ar) override;

LN_CONSTRUCT_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	bool init();
	bool init(MeshModel* model);
	bool init(const StringRef& filePath, MeshImportSettings* settings);

private:
    Ref<MeshComponent> m_component;
};

//==============================================================================
// StaticMesh::Builder

struct StaticMesh::BuilderDetails : public VisualObject::BuilderDetails
{
	LN_BUILDER_DETAILS(StaticMesh);

	void apply(StaticMesh* p) const;
};

template<class T, class B, class D>
struct StaticMesh::BuilderCore : public VisualObject::BuilderCore<T, B, D>
{
	LN_BUILDER_CORE(VisualObject::BuilderCore);
};

LN_BUILDER_IMPLEMENT(StaticMesh);

} // namespace ln

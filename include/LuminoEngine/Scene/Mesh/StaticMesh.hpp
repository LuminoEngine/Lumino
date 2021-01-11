
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class MeshModel;

/**
 * StaticMesh
 */
LN_CLASS()
class StaticMesh
	: public VisualObject
{
    LN_OBJECT;
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
	void init();
    void init(MeshModel* model);
    void init(const StringRef& filePath, MeshImportSettings* settings);

private:
    Ref<MeshComponent> m_component;
};

} // namespace ln

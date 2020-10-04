
#pragma once
#include "../VisualObject.hpp"

namespace ln {
    
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
	static Ref<StaticMesh> load(const StringRef& filePath);

    // obsolete
    static Ref<StaticMesh> create();
    static Ref<StaticMesh> create(StaticMeshModel* model);
    static Ref<StaticMesh> create(const StringRef& filePath, float scale = 1.0f);
    
	/**指定した名前の MeshContainer から、衝突判定用の Body を作成します。 */
	LN_METHOD()
	void makeCollisionBody(StringRef meshContainerName);

    StaticMeshComponent* staticMeshComponent() const;

protected:
    void serialize(Serializer2& ar) override;

LN_CONSTRUCT_ACCESS:
	StaticMesh();
	virtual ~StaticMesh();
	void init();
    void init(StaticMeshModel* model);
    void init(const StringRef& filePath, float scale);

private:
    Ref<StaticMeshComponent> m_component;
};

} // namespace ln

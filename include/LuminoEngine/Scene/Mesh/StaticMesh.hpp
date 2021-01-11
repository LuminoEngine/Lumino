
#pragma once
#include "../VisualObject.hpp"

namespace ln {
class MeshModel;

/**
 * Mesh
 */
LN_CLASS()
class Mesh
	: public VisualObject
{
    LN_OBJECT;
public:
	/** load */
	LN_METHOD()
	static Ref<Mesh> load(const StringRef& filePath);

    // obsolete
    static Ref<Mesh> create();
    static Ref<Mesh> create(MeshModel* model);
    static Ref<Mesh> create(const StringRef& filePath, float scale = 1.0f);
    
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
	Mesh();
	virtual ~Mesh();
	void init();
    void init(MeshModel* model);
    void init(const StringRef& filePath, float scale);

private:
    Ref<MeshComponent> m_component;
};

} // namespace ln


#pragma once
#include "../VisualObject.hpp"

namespace ln {

class StaticMesh
	: public VisualObject
{
    LN_OBJECT;
public:
    static Ref<StaticMesh> create();
    static Ref<StaticMesh> create(StaticMeshModel* model);
    static Ref<StaticMesh> create(const StringRef& filePath, float scale = 1.0f);

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

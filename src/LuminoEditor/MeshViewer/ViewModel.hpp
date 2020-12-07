#pragma once
#include "VisualizedMeshComponent.hpp"

class ViewModel
    : public ln::Object
{
public:
	static ViewModel* instance();

	void load();

	const Ref<ln::WorldObject>& meshObject() const { return m_meshObject; }
	const Ref<VisualizedMeshComponent>& meshComponent() const { return m_meshComponent; }

protected:

private:
	ViewModel();

	Ref<ln::MeshModel> m_model;
	Ref<VisualizedMeshComponent> m_meshComponent;
	Ref<ln::WorldObject> m_meshObject;
};


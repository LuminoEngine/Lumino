#pragma once

class ViewModel
    : public ln::Object
{
public:
	static ViewModel* instance();

	void load();

	const Ref<ln::SkinnedMesh>& mesh() const { return m_mesh; }

protected:

private:
	ViewModel();

	Ref<ln::SkinnedMesh> m_mesh;
};


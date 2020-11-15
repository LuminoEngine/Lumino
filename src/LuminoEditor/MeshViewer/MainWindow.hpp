#pragma once

class MainWindow
    : public ln::UIMainWindow
{
public:
	MainWindow();

protected:
	virtual void onLoaded() override;

private:
	Ref<ln::SkinnedMesh> m_mesh;
};


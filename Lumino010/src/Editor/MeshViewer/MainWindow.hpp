#pragma once

class MainWindow
    : public ln::UIMainWindow
{
public:
	MainWindow();

protected:
	virtual void onLoaded() override;

private:
	void handleImGUI(ln::UIEventArgs* e);

	Ref<ln::SkinnedMesh> m_mesh;
};


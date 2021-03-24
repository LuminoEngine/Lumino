#pragma once

namespace ln {
class ImGuiDockPane;

class Editor
{
public:
	static void addPane(ImGuiDockPane* pane);
	static void closeAllPanes();

	static ImGuiDockPane* editorViewportToolPane();
};

} // namespace ln


#pragma once

namespace ln {
class ImGuiDockPane;

class Editor
{
public:
	static void addPane(ImGuiDockPane* pane);

	static ImGuiDockPane* editorViewportToolPane();
};

} // namespace ln


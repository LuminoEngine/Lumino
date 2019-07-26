#pragma once

class NavigatorManager;
class DocumentManager;
class ToolPanesArea;
class OutputPane;
class ProblemsPane;
class InspectorPane;

class MainWindow
    : public ln::UIMainWindow
{
public:
	MainWindow();

protected:
	virtual void onLoaded() override;

private:
	Ref<NavigatorManager> m_navigatorManager;
	Ref<DocumentManager> m_documentManager;

	Ref<ToolPanesArea> m_toolPanesArea;
	Ref<ToolPanesArea> m_inspectorPanesArea;

	Ref<OutputPane> m_outputPane;
	Ref<ProblemsPane> m_problemsPane;
	Ref<InspectorPane> m_inspectorPane;
};


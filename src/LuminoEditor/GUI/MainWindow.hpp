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

    const Ref<ln::UISplitter>& mainHSplitter() const { return m_mainHSplitter; }

    const Ref<NavigatorManager>& navigatorManager() const { return m_navigatorManager; }
    const Ref<DocumentManager>& documentManager() const { return m_documentManager; }

    const Ref<ToolPanesArea>& toolPanesArea() const { return m_toolPanesArea; }
    const Ref<ToolPanesArea>& inspectorPanesArea() const { return m_inspectorPanesArea; }

    const Ref<OutputPane>& outputPane() const { return m_outputPane; }
    const Ref<ProblemsPane>& problemsPane() const { return m_problemsPane; }
    const Ref<InspectorPane>& inspectorPane() const { return m_inspectorPane; }

protected:
	virtual void onLoaded() override;

private:
    void onNavigationViewOpen();
    void onNavigationViewClose();

    Ref<ln::UISplitter> m_mainHSplitter;

	Ref<NavigatorManager> m_navigatorManager;
	Ref<DocumentManager> m_documentManager;

	Ref<ToolPanesArea> m_toolPanesArea;
	Ref<ToolPanesArea> m_inspectorPanesArea;

	Ref<OutputPane> m_outputPane;
	Ref<ProblemsPane> m_problemsPane;
	Ref<InspectorPane> m_inspectorPane;
};


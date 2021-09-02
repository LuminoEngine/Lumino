#pragma once

class ToolPaneContainer : public QFrame
{
    Q_OBJECT
public:
    explicit ToolPaneContainer(QWidget* parent = nullptr);

	void addStandalonePane(QFrame* pane);

private:
	QTabWidget* m_tabs;
};

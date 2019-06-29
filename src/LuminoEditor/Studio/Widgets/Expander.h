#pragma once

class Expander : public QWidget {
    Q_OBJECT
private:
	QGridLayout* m_rootLayout;
	QHBoxLayout* m_headerToolLayout;
	QToolButton* m_toggleButton;
	QFrame* m_contentArea;
	QVBoxLayout* m_contentLayout;
	QWidget* m_content;
	int m_contentHeight;

public:
    explicit Expander(QString title = "", QWidget* parent = nullptr);

	void addToolWidget(QWidget* widget);

    void setContent(QWidget* content);
};

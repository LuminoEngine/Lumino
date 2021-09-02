#pragma once

namespace lna {	class Project; }
class ContentsViewProvider;

class ContentsViewManager : public QObject
{
    Q_OBJECT
public:
    ContentsViewManager();

	void setup(lna::Project* project, QString toolsetName);

    lna::Project* project() const { return m_project; }
    QFrame* sidebar() { return m_sidebar; }
    QStackedWidget* viewContainer() { return m_viewContainer; }

    void addContentsViewProvider(ContentsViewProvider* provider);

    

protected:

private:
    void onContainerButtonCheckChanged(bool checked);

	struct ProviderInfo
	{
		ContentsViewProvider* provider;
		QPushButton* iconButton;
	};

    lna::Project* m_project;
    QFrame* m_sidebar;
    QVBoxLayout* m_buttonContainer;
    QStackedWidget* m_viewContainer;
    QList<ProviderInfo> m_providers;
};


class ContentsViewProvider : public QFrame
{
    Q_OBJECT
public:
    virtual QString icon() const = 0;
	virtual void onChangeCurrentProjet(lna::Project* project) = 0;

protected:
    ContentsViewProvider(QWidget* parent);
    ContentsViewManager* manager() const { return m_manager; }

private:
    ContentsViewManager* m_manager;

    friend class ContentsViewManager;
};

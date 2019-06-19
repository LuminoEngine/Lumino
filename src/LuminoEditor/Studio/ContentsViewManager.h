#pragma once

namespace lna {	class Project; }
class ContentsViewProvider;

class ContentsViewManager : public QObject
{
    Q_OBJECT
public:
    ContentsViewManager();

	void setup(QString toolsetName);

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

    QFrame* m_sidebar;
    QVBoxLayout* m_buttonContainer;
    QStackedWidget* m_viewContainer;
    QList<ProviderInfo> m_providers;
};


class ContentsViewProvider : public QObject
{
    Q_OBJECT
public:
    virtual QString icon() const = 0;
    virtual QWidget* createView() = 0;
	virtual void onChangeCurrentProjet(lna::Project* project) = 0;

protected:
    ContentsViewProvider(QObject* parent);

private:
};

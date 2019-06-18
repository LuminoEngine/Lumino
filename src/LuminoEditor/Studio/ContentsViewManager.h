#pragma once

class ContentsViewProvider;

class ContentsViewManager : public QObject
{
    Q_OBJECT
public:
    ContentsViewManager();
    QFrame* sidebar() { return m_sidebar; }
    QStackedWidget* viewContainer() { return m_viewContainer; }

    void addContentsViewProvider(ContentsViewProvider* provider);

    

protected:

private:
    void onContainerButtonCheckChanged(bool checked);

    QFrame* m_sidebar;
    QVBoxLayout* m_buttonContainer;
    QStackedWidget* m_viewContainer;
    QList<ContentsViewProvider*> m_providers;
};


class ContentsViewProvider : public QObject
{
    Q_OBJECT
public:
    virtual QString icon() const = 0;
    virtual QWidget* createView() = 0;

protected:
    ContentsViewProvider(QObject* parent);

private:
};

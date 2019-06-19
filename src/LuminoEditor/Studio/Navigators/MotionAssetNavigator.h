#pragma once
#include "../ContentsViewManager.h"
#include "../Widgets/Expander.h"


class MotionAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit MotionAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class MotionAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit MotionAssetTreeView(QWidget* parent = nullptr);

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    MotionAssetTreeModel* m_model;
};



class MotionContentsViewProvider : public ContentsViewProvider
{
public:
    MotionContentsViewProvider(QWidget* parent);
    virtual QString icon() const override { return "fileimageo"; }
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    MotionAssetTreeView* view() const { return m_treeView; }

private:
	QVBoxLayout* m_rootLayout;
	Expander* m_expander;
    MotionAssetTreeView* m_treeView;
};


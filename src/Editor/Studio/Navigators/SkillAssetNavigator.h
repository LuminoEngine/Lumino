#pragma once
#include "../ContentsViewManager.h"
#include "../Widgets/Expander.h"


class SkillAssetTreeModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit SkillAssetTreeModel(QObject* parent = nullptr)
        : QFileSystemModel(parent)
    {}
};

class SkillAssetTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit SkillAssetTreeView(QWidget* parent = nullptr);

    void setRootDir(QString dir);

public slots:
    void onDoubleClicked(const QModelIndex &index);

private:
    SkillAssetTreeModel* m_model;
};



class SkillContentsViewProvider : public ContentsViewProvider
{
public:
    SkillContentsViewProvider(QWidget* parent);
    virtual QString icon() const override { return "fileimageo"; }
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    SkillAssetTreeView* view() const { return m_treeView; }

private:
	QVBoxLayout* m_rootLayout;
	Expander* m_expander;
    SkillAssetTreeView* m_treeView;
};


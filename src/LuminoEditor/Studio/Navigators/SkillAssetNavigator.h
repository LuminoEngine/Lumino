#pragma once
#include "../ContentsViewManager.h"


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
    SkillContentsViewProvider(QObject* parent);
    virtual QString icon() const override { return "fileimageo"; }
    virtual QWidget* createView() override;
	virtual void onChangeCurrentProjet(lna::Project* project) override {}

    SkillAssetTreeView* view() const { return m_treeView; }

private:
    SkillAssetTreeView* m_treeView;
};


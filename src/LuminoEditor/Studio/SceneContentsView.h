#pragma once
#include "ContentsViewManager.h"

class SceneContentsViewProvider : public ContentsViewProvider
{
public:
    virtual ln::String icon() const override { return u"music"; }
    virtual QWidget* createView() override;

LN_CONSTRUCT_ACCESS:
    SceneContentsViewProvider();

private:
    SceneContentsViewProvider* m_treeView;
};


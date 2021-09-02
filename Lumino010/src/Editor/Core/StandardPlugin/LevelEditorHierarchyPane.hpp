#pragma once
#include "../App/ToolPanesArea.hpp"

namespace lna {
class LevelEditor;

class LevelEditorHierarchyPane
    : public ln::EditorPane
{
public:
    LevelEditorHierarchyPane();
    bool init(LevelEditor* model);

private:
    LevelEditor* m_levelEditorModel;
};

} // namespace lna 

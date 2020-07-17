#pragma once
#include "../App/ToolPanesArea.hpp"

namespace lna {
class LevelEditor;

class LevelEditorPropertyPane
    : public ln::EditorPane
{
public:
    LevelEditorPropertyPane();
    bool init(LevelEditor* model);

private:
    LevelEditor* m_levelEditorModel;
};

} // namespace lna 

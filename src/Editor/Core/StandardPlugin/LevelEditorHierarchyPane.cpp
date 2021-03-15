
#include "LevelEditorHierarchyPane.hpp"
#include "LevelEditor.hpp"

namespace lna {
   
//==============================================================================
// LevelEditorHierarchyPane

LevelEditorHierarchyPane::LevelEditorHierarchyPane()
	: m_levelEditorModel(nullptr)
{
}

bool LevelEditorHierarchyPane::init(LevelEditor* model)
{
	if (!EditorPane::init()) return false;
	m_levelEditorModel = model;

	setBackgroundColor(ln::Color::Red);

	return true;
}

} // namespace lna


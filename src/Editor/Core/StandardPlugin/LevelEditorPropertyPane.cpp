
#include <LuminoEngine/UI/Controls/UIPropertyFields.hpp>
#include "LevelEditorPropertyPane.hpp"
#include "LevelEditor.hpp"

namespace lna {
   
//==============================================================================
// LevelEditorPropertyPane

LevelEditorPropertyPane::LevelEditorPropertyPane()
	: m_levelEditorModel(nullptr)
{
}

bool LevelEditorPropertyPane::init(LevelEditor* model)
{
	if (!EditorPane::init()) return false;
	m_levelEditorModel = model;

	//setBackgroundColor(ln::Color::Red);


	auto mainLauout = ln::makeObject<ln::UIGridLayout>();
	mainLauout->setName(_TT("PropertyPane.mainLauout"));
	mainLauout->setColumnCount(2);
#if LN_USE_YOGA
    LN_NOTIMPLEMENTED();
#else
	mainLauout->setVAlignment(ln::UIVAlignment::Top);
#endif
	addChild(mainLauout);

	// FogStartDistance
	{
		auto text1 = ln::UIText::create(_TT("FogStartDistance"));
		mainLauout->addChild(text1);

		auto field1 = ln::makeObject<ln::UISliderField>();
		field1->setMaximum(1000);
		field1->setChanged([this](float v) { m_levelEditorModel->editingLevel()->acquireRenderParameters()->m_fogStartDistance = v; });
		mainLauout->addChild(field1);
	}

	// FogStartDistance
	{
		auto text1 = ln::UIText::create(_TT("FogDensity"));
		mainLauout->addChild(text1);

		auto field1 = ln::makeObject<ln::UISliderField>();
		field1->setChanged([this](float v) { m_levelEditorModel->editingLevel()->acquireRenderParameters()->m_fogDensity = v; });
		mainLauout->addChild(field1);
	}


	return true;
}

} // namespace lna


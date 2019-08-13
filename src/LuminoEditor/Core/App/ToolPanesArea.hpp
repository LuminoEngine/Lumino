#pragma once

namespace ln {

class EditorPane
	: public ln::UIControl
{
public:
    EditorPane();


protected:

private:
};

} // namespace ln

class ToolPanesArea
    : public ln::UIControl
{
public:
	ToolPanesArea();
    void init();

	void addPane(ln::EditorPane* pane);

protected:

private:

};


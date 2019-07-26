#pragma once

class ToolPane
	: public ln::UIControl
{
public:
	ToolPane();


protected:

private:
};

class ToolPanesArea
    : public ln::UIControl
{
public:
	ToolPanesArea();

	void addPane(ToolPane* pane);

protected:

private:

};


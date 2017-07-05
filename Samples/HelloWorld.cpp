#include <Lumino.h>
using namespace ln;

void Main_HelloWorld()
{
	auto size = UIViewport::getMain()->getViewSize();

	auto text = TextBlock2D::create(_T("Hello, Lumino!"));
	text->setPosition(size.width / 2, size.height / 2);
	text->setAnchorPoint(0.5, 0.5);

	while (Engine::update())
	{
	}
}


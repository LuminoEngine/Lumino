#include <Lumino.h>
using namespace ln;

void Main_HelloWorld()
{
	auto text = TextBlock2D::create(_T("Hello, Lumino!"));
	auto size = UIViewport::getMain()->getViewSize();

	text->setPosition(size.width / 2, size.height / 2);

	while (Engine::update())
	{
		//printf("----\n");
	}
}


#include <Lumino.hpp>

void Main()
{
	Engine::init();

	auto text = UITextBlock::create(u"Hello, Lumino!");

	UI::add(text);

	while (Engine::update()) {

	}
}

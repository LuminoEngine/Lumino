#include <Lumino.h>
using namespace ln;


//class UISampleListButton : public UIButton
//{
//public:
//
//private:
//};



void Main_HelloWorld();
void Main_Sprite2D();

struct SampleInfo
{
	String				name;
	Delegate<void()>	mainFunc;
};

static SampleInfo g_samples[] =
{
	{ _T("HelloWorld"), Main_HelloWorld },
	{ _T("Sprite2D"), Main_Sprite2D },
};

int g_samplesIndex = 0;

void Main()
{
	Engine::initialize();


	auto* mainWindow = Engine::getMainWindow();

	auto button1 = UIToggleButton::create();
	button1->setSize(Size(32, 32));
	mainWindow->addChild(button1);


	auto listBox1 = UIListBox::create();
	listBox1->setWidth(200);
	//mainWindow->addChild(listBox1);

	for (int i = 0; i < LN_ARRAY_SIZE_OF(g_samples); i++)
	{
		listBox1->addTextItem(g_samples[i].name);
	}


	while (!Engine::isEndRequested() && g_samplesIndex >= 0)
	{
		g_samples[g_samplesIndex].mainFunc();
	}

	Engine::terminate();
}


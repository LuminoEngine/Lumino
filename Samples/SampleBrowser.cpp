#include <Lumino.h>
using namespace ln;

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


	auto* mainWindow = Engine::GetMainWindow();


	auto listBox1 = UIListBox::create();
	listBox1->SetWidth(200);
	mainWindow->AddChild(listBox1);

	for (int i = 0; i < LN_ARRAY_SIZE_OF(g_samples); i++)
	{
		listBox1->AddTextItem(g_samples[i].name);
	}


	while (!Engine::IsEndRequested() && g_samplesIndex >= 0)
	{
		g_samples[g_samplesIndex].mainFunc();
	}

	Engine::Terminate();
}


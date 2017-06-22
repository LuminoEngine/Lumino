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




	auto clock = AnimationClock::create();

	auto listBox1 = UIListBox::create();
	listBox1->setWidth(200);
	listBox1->margin = ThicknessF(0, 32, 0, 0);
	for (int i = 0; i < LN_ARRAY_SIZE_OF(g_samples); i++)
	{
		listBox1->addTextItem(g_samples[i].name);
	}


	auto listWindow = UIUserControl::create();
	listWindow->setWidth(220);
	listWindow->setBackground(SolidColorBrush::Blue);
	listWindow->setAnchor(AlignmentAnchor::TopOffsets | AlignmentAnchor::BottomOffsets);	// TODO: UIAnchor ‚Å‚¢‚¢‚Æ‚¨‚à‚¤
	listWindow->addChild(listBox1);
	mainWindow->addChild(listWindow);


	auto button1 = UIToggleButton::create();
	button1->setSize(Size(32, 32));
	button1->connectOnChecked([&](UIEventArgs* e)
	{
		// open
		button1->setText(_T("<"));
		clock->start(listWindow->getPosition().x, 0, 1.0, EasingMode::EaseOutQuad, [listWindow](float v) {listWindow->setPosition(PointF(v, 0)); }, nullptr);
	});
	button1->connectOnUnchecked([&](UIEventArgs* e)
	{
		// close
		button1->setText(_T(">"));
		clock->start(listWindow->getPosition().x, -200, 1.0, EasingMode::EaseOutQuad, [listWindow](float v) {listWindow->setPosition(PointF(v, 0)); }, nullptr);
	});
	button1->setText(_T(">"));
	button1->setBackground(SolidColorBrush::White);
	mainWindow->addChild(button1);




	Engine::resetFrameDelay();

	while (!Engine::isEndRequested() && g_samplesIndex >= 0)
	{
		g_samples[g_samplesIndex].mainFunc();

	}

	Engine::terminate();
}


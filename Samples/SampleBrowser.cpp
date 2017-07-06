#include <Lumino.h>
using namespace ln;

namespace sample {

struct SampleInfo
{
	String			caption;
	SampleMainFunc	main;
};

List<SampleInfo>	g_samples;

SampleMainFunc registerSample(const char* name, SampleMainFunc func)
{
	SampleInfo info;
	info.caption = String::fromNativeCharString(name);
	info.main = func;
	g_samples.add(info);
	return func;
}

} // namespace sample


void Main_HelloWorld();
void Main_Sprite2D();
void Main_DebugAndTestSupport();

struct SampleInfo
{
	String				name;
	Delegate<void()>	mainFunc;
};

static SampleInfo g_samples[] =
{
	{ _T("HelloWorld"), Main_HelloWorld },
	{ _T("Sprite2D"), Main_Sprite2D },
	{ _T("DebugAndTestSupport"), Main_DebugAndTestSupport },
};

int						g_samplesIndex = 0;
RefPtr<UIToggleButton>	g_pinButton;
RefPtr<UIUserControl>	g_listWindow;
RefPtr<AnimationClock>	g_clock;

void showListWindow()
{
	g_pinButton->setText(_T("<"));
	g_clock->start(g_listWindow->getPosition().x, 0, 0.5, EasingMode::EaseOutExpo, [](float v) {g_listWindow->setPosition(PointF(v, 0)); }, nullptr);
}

void closeListWindow()
{
	g_pinButton->setText(_T(">"));
	g_clock->start(g_listWindow->getPosition().x, -200, 0.5, EasingMode::EaseOutExpo, [](float v) {g_listWindow->setPosition(PointF(v, 0)); }, nullptr);
}

void Main()
{
	Engine::initialize();


	auto* mainWindow = Application::getMainWindow();


	g_clock = AnimationClock::create();

	auto listBox1 = UIListBox::create();
	listBox1->setWidth(200);
	listBox1->margin = ThicknessF(0, 32, 0, 0);
	for (int i = 0; i < LN_ARRAY_SIZE_OF(g_samples); i++)
	{
		listBox1->addTextItem(g_samples[i].name);
	}


	g_listWindow = UIUserControl::create();
	g_listWindow->setWidth(220);
	g_listWindow->setBackground(SolidColorBrush::Blue);
	g_listWindow->setAnchor(AlignmentAnchor::TopOffsets | AlignmentAnchor::BottomOffsets);	// TODO: UIAnchor ‚Å‚¢‚¢‚Æ‚¨‚à‚¤
	g_listWindow->addChild(listBox1);
	mainWindow->addChild(g_listWindow);


	g_pinButton = UIToggleButton::create();
	g_pinButton->setSize(Size(32, 32));
	g_pinButton->connectOnChecked([&](UIEventArgs* e) { showListWindow(); });
	g_pinButton->connectOnUnchecked([&](UIEventArgs* e) { closeListWindow(); });
	g_pinButton->setBackground(SolidColorBrush::White);
	mainWindow->addChild(g_pinButton);

	//g_pinButton->setChecked(false);
	closeListWindow();

	//showListWindow();


	Engine::resetFrameDelay();

	while (!Engine::isEndRequested() && g_samplesIndex >= 0)
	{
		g_samples[g_samplesIndex].mainFunc();

	}

	Engine::terminate();
}


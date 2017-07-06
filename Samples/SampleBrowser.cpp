#include <Lumino.h>
using namespace ln;

struct SampleInfo
{
	String			caption;
	sample::SampleMainFunc	mainFunc;
};

class SampleManager
{
public:
	// VS2017 ではグローバル変数で std::vector や ln::List を作ると main 始まる前にデストラクタが呼ばれて空っぽになってしまった。バグかな？
	// ローカルの static 変数なら大丈夫なので、gtest と同じ仕組みでシングルトンを実装する。
	static SampleManager* getInstance();

public:
	std::vector<SampleInfo>	m_samples;
};


SampleManager* SampleManager::getInstance()
{
	static SampleManager instance;
	return &instance;
}




namespace sample {
bool g_sceneChanging = false;


SampleMainFunc registerSample(const char* name, SampleMainFunc func)
{
	SampleInfo info;
	info.caption = String::fromNativeCharString(name);
	info.mainFunc = func;
	SampleManager::getInstance()->m_samples.push_back(info);
	return func;
}

} // namespace sample

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
	//listBox1->margin = ThicknessF(0, 32, 0, 0);
	for (int i = 0; i < SampleManager::getInstance()->m_samples.size(); i++)
	{
		listBox1->addTextItem(SampleManager::getInstance()->m_samples[i].caption);
	}


	g_listWindow = UIUserControl::create();
	//g_listWindow->setWidth(220);
	g_listWindow->setBackground(UIColors::getBrush(UIColorIndex::Grey, 3));
	g_listWindow->setAnchor(AlignmentAnchor::TopOffsets | AlignmentAnchor::BottomOffsets);	// TODO: UIAnchor でいいとおもう
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
		SampleManager::getInstance()->m_samples[g_samplesIndex].mainFunc();

	}

	Engine::terminate();
}


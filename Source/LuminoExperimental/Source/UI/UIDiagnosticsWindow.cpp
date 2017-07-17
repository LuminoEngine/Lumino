
#include <Lumino/Graphics/FrameCapturer.h>
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/UI/UIStyle.h>
#include <Lumino/UI/UIButton.h>
#include <Lumino/Profiler.h>
#include <LuminoExperimental/UI/UIDiagnosticsWindow.h>

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
UIDiagnosticsWindow::UIDiagnosticsWindow()
	: UIFrameWindow()
	, m_font(nullptr)
{
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::initialize()
{
	UIFrameWindow::initialize();
	m_font = Font::getBuiltin(BuiltinFontSize::XXSmall);

	m_recordButton = UIToggleButton::create(_T("Record"), 100, 24);
	m_recordButton->setPosition(Point(100, 200));
	m_recordButton->connectOnChecked([](UIEventArgs* e) { FrameCapturer::startRecording(); });	// TODO: "UIEventArgs* e" を描くのがめんどい
	m_recordButton->connectOnUnchecked([](UIEventArgs* e) { FrameCapturer::stopRecording(); });
	addChild(m_recordButton);
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::onRender(DrawingContext* context)
{
	UIFrameWindow::onRender(context);
	//context->clear(ClearFlags::All, Color::AliceBlue);
	drawStatistics(context, Rect(640 - 8 - 300, 8, 300, 256));
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::drawStatistics(DrawingContext* context, const Rect& windowRect)
{
	Point loc(windowRect.x, windowRect.y);


	// キャプションバー
	//context->setOpacity(0.5f);
	//context->setBrush(SolidColorBrush::DimGray);
	//context->drawRectangle(Rect(windowRect.getTopLeft(), windowRect.width, 20));

	//context->setOpacity(1.0f);
	context->setFont(m_font);
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 8));
	context->drawText_(_T("Statistics"), Point(10, 20));

	//return;
	loc.y += 24;

	//auto time = Profiler2::getSection(ProfilingKeys::Engine_UpdateFrame)->getElapsedSeconds();
	//printf("%f\n", time);

	//-----------------------------------------------------
	// Main info
	context->drawText_(_T("Main information:"), Point(loc.x + 8, loc.y));
	loc.y += 16;
	loc.x += 16;

	TCHAR text[256] = { 0 };

	//StringTraits::sprintf(text, 256, _T("Graphics API    : %s"), m_manager->GetGraphicsAPI().ToString().c_str());
	//context->drawText_(text, loc);
	//loc.y += 16;

	//StringTraits::sprintf(text, 256, _T("Rendering type  : %s"), m_manager->GetRenderingType().ToString().c_str());
	//context->drawText_(text, loc);
	//loc.y += 16;

	StringTraits::sprintf(text, 256, _T("Average FPS     : %.1f"), Profiler2::GetMainFps());
	context->drawText_(text, loc);

	StringTraits::sprintf(text, 256, _T(" / Capacity : %.1f"), Profiler2::GetMainFpsCapacity());
	context->drawText_(text, Point(loc.x + 150, loc.y));
	loc.y += 16;

	//StringTraits::sprintf(text, 256, _T("Window Size     : %d x %d"), m_profiler->GetCommitedMainWindowSize().Width, m_profiler->GetCommitedMainWindowSize().Height);
	//context->drawText_(text, loc);
	//loc.y += 16;

	//StringTraits::sprintf(text, 256, _T("Backbuffer Size : %d x %d"), m_profiler->GetCommitedMainBackbufferSize().Width, m_profiler->GetCommitedMainBackbufferSize().Height);
	//context->drawText_(text, loc);

	loc.y += 16;
	loc.x -= 16;

	//-----------------------------------------------------
	// Threads performance

	context->drawText_(_T("Threads performance:"), Point(loc.x + 8, loc.y));
	loc.y += 16;

	// グループリストの描画
	Rect listRect(loc.x + 16, loc.y, windowRect.width - 32, windowRect.height - loc.y);
	drawGroupList(context, listRect);
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::drawGroupList(DrawingContext* context, const Rect& listRect)
{

	ProfilingKey* groups[] =
	{
		ProfilingKeys::Engine,
		ProfilingKeys::Rendering,
	};
	int groupCount = 2;

	// 名前カラム背景色
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 1));
	//context->setOpacity(0.2f);
	context->drawRectangle(Rect(listRect.getTopLeft(), ThreadNameColumnWidth, (float)(groupCount * rowHeight)));

	// バーのNG領域背景色
	Rect ngArea(
		listRect.x + (ThreadNameColumnWidth + SafeAreaWidth),
		listRect.y,
		listRect.width - (ThreadNameColumnWidth + SafeAreaWidth),
		groupCount * rowHeight);
	context->setBrush(UIColors::getBrush(UIColorIndex::Red, 2));
	context->drawRectangle(ngArea);

	// リスト縦罫線
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
	//context->SetOpacity(0.7f);
	context->drawRectangle(Rect(listRect.x + ThreadNameColumnWidth - 1, listRect.y, 1, groupCount * rowHeight));

	int iGrout;
	for (iGrout = 0; iGrout < groupCount; iGrout++)
	{
		// row 上部罫線
		context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
		//context->SetOpacity(0.7f);
		context->drawRectangle(Rect(listRect.x, listRect.y + (iGrout * rowHeight), listRect.width, 1.0f));

		// グループ名
		context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 8));
		//context->SetOpacity(1.0f);
		Point pt(listRect.x + 4, listRect.y + (iGrout * rowHeight) + 1);
		context->drawText_(groups[iGrout]->getName(), pt);

		// セクションを積み上げ棒グラフで表示
		float totalElapsed = 0;
		drawSectionGraphBar(context, groups[iGrout], listRect.x + ThreadNameColumnWidth, listRect.y + (iGrout * rowHeight), listRect, &totalElapsed);

		// ms
		TCHAR fps[256] = { 0 };
		StringTraits::sprintf(fps, 256, _T("%.1f ms"), totalElapsed * 1000.0f);
		pt.y += 16;
		context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
		context->drawText_(fps, pt);
	}

	// row 下部罫線
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
	//context->SetOpacity(0.7f);
	context->drawRectangle(Rect(listRect.x, listRect.y + (groupCount * rowHeight), listRect.width, 1.0f));
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::drawSectionGraphBar(DrawingContext* context, ProfilingKey* key, float x, float y, const Rect& listRect, float* totalElapsed)
{
	float time = 0;
	float nextX = 0;
	int colorIndex = (int)UIColorIndex::LightBlue;
	for (const auto childKey : key->getChildren())
	{
		auto* section = Profiler2::getSection(childKey);

		//if (section->getElapsedNanoseconds() > 0.0)
		{
			float ratio = section->getElapsedSeconds() / m_limitElapsedTime;
			time += section->getElapsedSeconds();

			Rect srcRect(x + nextX, y + 5, ratio * SafeAreaWidth, rowHeight - 10);
			if (listRect.x + listRect.width < srcRect.x + srcRect.width)
			{
				srcRect.width = listRect.x + listRect.width - srcRect.x;
			}

			context->setBrush(UIColors::getBrush((UIColorIndex)colorIndex, 2));
			context->drawRectangle(srcRect);

			nextX += ratio * SafeAreaWidth;
			if (listRect.x + listRect.width <= nextX) { break; }
		}
		
		colorIndex += 4;
		if (colorIndex > (UIColors::MaxIndex - 2))
		{
			colorIndex = 0;
		}
	}

	*totalElapsed = time;
}

LN_NAMESPACE_END

#pragma once
#include "../UIContainerElement.hpp"
#include "../UIAdorner.hpp"

namespace ln {
class UIButton;
class UIDialogAdorner;
class UIBoxLayout;

enum class UIDialogButtonRole
{
	None,

	/** Clicking the button causes the dialog to be accepted. (e.g. OK, Yes) */
	Accept,

	/** Clicking the button causes the dialog to be rejected. (e.g. Cancel) */
	Reject,

	/** (e.g. No) */
	Discard,

	/** The button applies current changes. */
	Apply,
};

enum class UIDialogButtons
{
	OK,
	OKCancel,
	YesNo,
	YesNoCancel,
};

class UIDialog
    : public UIContainerElement
{
public:
    bool isOpend() const { return m_opend; }

    void open();
    void close();

	void setupDialogButtons(UIDialogButtons buttons);

protected:
    virtual const String& elementName() const  override { static String name = _TT("UIDialog"); return name; }
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
	UIDialog();
    void init();

private:
	struct DialogButton
	{
		UIDialogButtonRole role;
		Ref<UIButton> button;
	};

	void addDialogButton(UIDialogButtonRole role, const String& text);
	void handleDialogButtonClicked();

    Ref<UIAdorner> m_adorner;
	List<DialogButton> m_dialogButtons;
	Ref<UIBoxLayout> m_dialogButtonsLayout;
    bool m_opend;

    friend class UIDialogAdorner;
};

// Backdrop も兼ねる
//
// モーダルダイアログ（ポップアップ）が複数表示されている場合は、UIPopupAdorner も複数表示される。
// Material-UI と同じ動作。
// つまり、画面全体を覆うように半透明グレーで覆うが、それがどんどん深くなる。
// - UIDialogAdorner
// - UIDialog
// - UIDialogAdorner
// - UIDialog
// というように。
// オーバーレイの色もどんどん濃くなる。
class UIDialogAdorner
    : public UIAdorner
{
protected:
	virtual void onRoutedEvent(UIEventArgs* e) override;
    virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
    virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;
    virtual void onUpdateLayout(UILayoutContext* layoutContext) override;
	virtual UIElement* lookupMouseHoverElement(const Point& frameClientPosition) override;
    virtual void render(UIRenderingContext* context, const Matrix& parentTransform) override;

LN_CONSTRUCT_ACCESS:
	UIDialogAdorner();
    void init(UIDialog* popup);

private:
    Ref<UIDialog> m_popup;
};

//class UIBackdrop
//	: public UIElement
//{
//public:
//	
//LN_CONSTRUCT_ACCESS:
//	UIBackdrop();
//    void init();
//
//private:
//};

} // namespace ln


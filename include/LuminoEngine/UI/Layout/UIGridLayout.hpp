#pragma once
#include "UILayoutPanel.hpp"

namespace ln {


enum class UILayoutRule
{
	Box,
	VerticalFlow,

	// 子要素を上詰めで配置する。
	// Row は Auto となり、Height 0 の Item は表示されない。
	HorizontalFlow,
};




template<class T, class B = typename T::Builder, typename... TArgs>
B makeDecl(TArgs&&... args)
{
	return typename T::Builder(std::forward<TArgs>(args)...);
}



/**
 * 行と列から構成されるグリッド状に UI 要素をレイアウトします。
 *
 */
LN_CLASS()
class UIGridLayout
    : public UILayoutPanel
{
	LN_OBJECT;
public:
	/*
	class Builder : public UILayoutPanel::Builder
	{
	public:
		LN_BUILDER(UIGridLayout);

		Builder& columnCount(int value);
		Builder& add(const UIElement::Builder& value);

		//Builder& children(std::initializer_list<UIElement::Builder> list)
		//{
		//	for (auto& p : list) add(p);
		//	return *this;
		//}

		//template<class T>
		//X fw(T const& t) { X x; f(x, t); return x; }


		//template<typename F, typename First, typename... Rest>
		//void each(F f, First firstValue, Rest... rest) {
		//	f(firstValue);
		//	each(f, rest...);
		//}

		//template <typename ... Args>
		//Builder& operator[](Args ... args)
		//{
		//	std::vector<UIElement::Builder> a = { args... };
		//	//each([this](auto& x) { add(x); }, args...);
		//	//UIElement::Builder a[] = { ... };
		//	for (auto& p : a) add(p);
		//	return *this;
		//}

		//template <class ...Args>
		////Builder& foo_imp(const Args&... args)
		//Builder& operator[](const Args&... args)
		//{
		//	std::vector<int> a = { args... };
		//	//for (auto& p : a) add(p);
		//	return *this;
		//}

	protected:
		class Details : public UILayoutPanel::Builder::Details
		{
		public:
			int columnCount;
			//std::vector<UIElement::Builder> children;
			virtual Ref<Object> build() override;
		};

	};
	*/

	void setRule(UILayoutRule value);

	/** setColumnCount */
	LN_METHOD(Property)
	void setColumnCount(int value);

	/** setRow */
	LN_METHOD()
	static void setRow(UIElement* element, int row);

	/** setColumn */
	LN_METHOD()
	static void setColumn(UIElement* element, int column);
	
	/** setPlacement */
	LN_METHOD()
	static void setPlacement(UIElement* element, int row, int column, int rowSpan = 1, int columnSpan = 1);

protected:
	virtual void onAddChild(UIElement* child) override;
	virtual Size measureOverride(UILayoutContext* layoutContext, const Size& constraint) override;
	virtual Size arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) override;

LN_CONSTRUCT_ACCESS:
	UIGridLayout();

	/** init */
	LN_METHOD()
	bool init();

private:
	struct GridDefinitionData
	{
		// input data
		UILayoutLengthType	type = UILayoutLengthType::Ratio;//UILayoutLengthType::Ratio;
		float			size = 0.0f;
		float			minSize = 0.0f;
		float			maxSize = std::numeric_limits<float>::max();

		// working data
		float			desiredSize = 0.0f;
		float			actualOffset = 0.0f;	// 最終オフセット
		float			actualSize = 0.0f;		// 最終サイズ

		float getAvailableDesiredSize() const
		{
			if (type == UILayoutLengthType::Auto) {
				return desiredSize;
			}
			else if (type == UILayoutLengthType::Direct) {
				return Math::clamp(size, minSize, maxSize);
			}
			else {
				return desiredSize;
			}
		}

		float getRatioSize() const
		{
			return (size == 0.0f) ? 1.0f : size;
		}

		void adjustActualSize()
		{
			actualSize = Math::clamp(actualSize, minSize, maxSize);
		}
	};

	int actualColumnCount() const;
	UILayoutLengthType defaultRowLengthType() const;
	UILayoutLengthType defaultColumnLengthType() const;

	List<GridDefinitionData> m_rowDefinitions;
	List<GridDefinitionData> m_columnDefinitions;
	UILayoutRule m_rule;
	int m_columnCount;
	int m_nextCellIndex;
};

} // namespace ln


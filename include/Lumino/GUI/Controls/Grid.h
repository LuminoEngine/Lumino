
#pragma once
#include "../GUIObjectCollection.h"
#include "../ContentElement.h"
#include "Panel.h"

namespace Lumino
{
namespace GUI
{
class Grid;

/**
	@brief		
*/
class ColumnDefinition
	: public ContentElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(ColumnDefinition);	// TODO マクロ名変えた方が良い気がする
public:
	static const PropertyID	WidthProperty;
	static const PropertyID	MinWidthProperty;
	static const PropertyID	MaxWidthProperty;

public:
	ColumnDefinition(GUIManager* manager);
	virtual ~ColumnDefinition();

	//-------------------------------------------------------------------------
	/** @name Properties *//** @{ */

	void SetWidth(float width) { m_width = width; }
	float GetWidth() const { return m_width; }

	void SetMinWidth(float width) { m_minWidth = width; }
	float GetMinWidth() const { return m_minWidth; }

	void SetMaxWidth(float width) { m_maxWidth = width; }
	float GetMaxWidth() const { return m_maxWidth; }

	/** @} */

public:
	bool IsAuto() const { return Math::IsNaN(m_width); }	// 個要素にあわせるか
	bool IsStar() const { return Math::IsInf(m_width); }	// レイアウト後、残りの領域を使うか
	bool IsPixel() const { return !IsAuto() && !IsStar(); }

private:
	float	m_width;
	float	m_minWidth;
	float	m_maxWidth;

	friend class Grid;
	float				m_actualWidth;
	float				m_actualOffsetX;
	Array<UIElement*>	m_elementGroup;		///< Measure/Arrange の作業用変数
	float				m_desiredWidth;		///< Measure/Arrange の作業用変数
	float GetAvailableDesiredWidth() const;	///< 最小/最大を考慮した幅を返す
};

/**
	@brief		
*/
class RowDefinition
	: public ContentElement
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(RowDefinition);	// TODO マクロ名変えた方が良い気がする
public:
	static const PropertyID	HeightProperty;
	static const PropertyID	MaxHeightProperty;
	static const PropertyID	MinHeightProperty;

public:
	RowDefinition(GUIManager* manager);
	virtual ~RowDefinition();

	bool IsAuto() const { return Math::IsNaN(m_height); }
	bool IsStar() const { return Math::IsInf(m_height); }
	bool IsPixel() const { return !IsAuto() && !IsStar(); }

public:
	//-------------------------------------------------------------------------
	/** @name Properties *//** @{ */

	void SetHeight(float height) { m_height = height; }
	float GetHeight() const { return m_height; }

	void SetMinHeight(float height) { m_minHeight = height; }
	float GetMinHeight() const { return m_minHeight; }

	void SetMaxHeight(float height) { m_maxHeight = height; }
	float GetMaxHeight() const { return m_maxHeight; }

	/** @} */

private:
	float	m_height;
	float	m_minHeight;
	float	m_maxHeight;

	friend class Grid;
	float				m_actualHeight;
	float				m_actualOffsetY;
	Array<UIElement*>	m_elementGroup;			///< Measure/Arrange の作業用変数
	float				m_desiredHeight;		///< Measure/Arrange の作業用変数
	float GetAvailableDesiredHeight() const;	///< 最小/最大を考慮した幅を返す
};

typedef GUIObjectList<ColumnDefinition>	ColumnDefinitionList;
typedef GUIObjectList<RowDefinition>	RowDefinitionList;

/**
	@brief		
*/
class Grid
	: public Panel
{
	LN_CORE_OBJECT_TYPE_INFO_DECL();
	LN_UI_ELEMENT_SUBCLASS_DECL(Grid);
public:
	static const PropertyID		ColumnDefinitionsProperty;
	static const PropertyID		RowDefinitionsProperty;

	static const AttachedProperty*	ColumnProperty;
	static AttachedProperty*	ColumnSpanProperty;
	static AttachedProperty*	RowProperty;
	static AttachedProperty*	RowSpanProperty;

public:
	Grid(GUIManager* manager);
	virtual ~Grid();

public:
	//-------------------------------------------------------------------------
	/** @name Properties *//** @{ */

	/// この Grid で定義されている ColumnDefinition のリストを取得します。
	ColumnDefinitionList* GetColumnDefinitions() const { return m_columnDefinitionList; }

	/// この Grid で定義されている RowDefinition のリストを取得します。
	RowDefinitionList* GetRowDefinitions() const { return m_rowDefinitionList; }

	/** @} */

public:
	//-------------------------------------------------------------------------
	/** @name Attached properties *//** @{ */

	/// ColumnProperty の値を指定した UIElement に設定します。
	static void SetColumn(UIElement* element, int value);

	/// ColumnProperty の値を指定した UIElement から取得します。
	static int GetColumn(UIElement* element);

	/// ColumnSpanProperty の値を指定した UIElement に設定します。
	static void SetColumnSpan(UIElement* element, int value);

	/// ColumnSpanProperty の値を指定した UIElement から取得します。
	static int GetColumnSpan(UIElement* element);

	/// RowProperty の値を指定した UIElement に設定します。
	static void SetRow(UIElement* element, int value);

	/// RowProperty の値を指定した UIElement から取得します。
	static int GetRow(UIElement* element);

	/// RowSpanProperty の値を指定した UIElement に設定します。
	static void SetRowSpan(UIElement* element, int value);

	/// RowSpanProperty の値を指定した UIElement から取得します。
	static int GetRowSpan(UIElement* element);

	/** @} */

protected:
	virtual void MeasureLayout(const SizeF& availableSize);
	virtual void ArrangeLayout(const RectF& finalLocalRect);

private:
	RefPtr<ColumnDefinitionList>	m_columnDefinitionList;
	RefPtr<RowDefinitionList>		m_rowDefinitionList;
};

} // namespace GUI
} // namespace Lumino

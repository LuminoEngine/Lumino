
#include "Internal.hpp"
#include <LuminoEngine/UI/Controls/UIScrollView.hpp> // for Thumb
#include <LuminoEngine/UI/Controls/UISplitter.hpp>

namespace ln {

//==============================================================================
// UISplitter

UISplitter::UISplitter()
	: m_gap(6.0f)
{
}

void UISplitter::init()
{
    UIControl::init();
}

void UISplitter::setOrientation(UILayoutOrientation value)
{
	if (m_cellDefinitions.size() > 0) {
		LN_NOTIMPLEMENTED();
	}
	m_orientation = value;
}

void UISplitter::setCellDefinition(int index, UILayoutLengthType type, float size, float minSize, float maxSize)
{
	if (m_cellDefinitions.size() <= index) {
		m_cellDefinitions.resize(index + 1);
	}
	CellDefinition& cell = m_cellDefinitions[index];
	cell.type = type;
    cell.size = size;
	cell.minSize = minSize;
	cell.maxSize = maxSize;

    cell.actualSize = Math::NaN;    // re-layout
}

void UISplitter::resetCellSizes()
{
	for (auto& cell : m_cellDefinitions) {
		cell.actualSize = Math::NaN;
	}
}

void UISplitter::setBarGap(float value)
{
	m_gap = value;
}

//void UISplitter::resetCellSize(int index)
//{
//    //if (m_cellDefinitions.size() <= index) {
//    //    m_cellDefinitions.resize(index + 1);
//    //}
//    //CellDefinition& cell = m_cellDefinitions[index];
//    //cell.actualSize = Math::NaN;
//}

UIElement* UISplitter::lookupMouseHoverElement(const Point& frameClientPosition)
{
	const auto localPos = frameClientPositionToLocalPosition(frameClientPosition);
	UIThumb* thumb;
	float distance;
	if (findNearThumb(localPos, &thumb, &distance)) {
		// Gap 内の場合は、Thumb へマウスイベントを流すようにすることで D&D できるようにする
		if (distance <= m_gap / 2) {
			return thumb;
		}
	}

	return UIControl::lookupMouseHoverElement(frameClientPosition);
}

void UISplitter::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
    int thumbCount = logicalChildren()->size() - 1;
    int diff = thumbCount - m_thumbs.size();
    for (int i = 0; i < diff; i++) {
        auto thumb = makeObject<UIThumb>();

		LN_NOTIMPLEMENTED();
		// TODO: style (-H, -V とかで分ける。)
		//thumb->addClass(_TT("SplitterBar"));
		//if (isHorizontal()) {
		//	thumb->setHAlignment(UIHAlignment::Stretch);
		//	thumb->setVAlignment(UIVAlignment::Stretch);
		//}
		//else {
		//	thumb->setHAlignment(UIHAlignment::Stretch);
		//	thumb->setVAlignment(UIVAlignment::Stretch);
		//}

        addVisualChild(thumb);
        m_thumbs.add(thumb);
    }

    return UIControl::onUpdateStyle(styleContext, finalStyle);
}

Size UISplitter::measureOverride(UILayoutContext* layoutContext, const Size& constraint) {
    LN_NOTIMPLEMENTED();
    return Size();
	//// Create missing cells.
	//auto& children = logicalChildren();
	//if (m_cellDefinitions.size() < children->size()) {
	//	m_cellDefinitions.resize(children->size());
	//}

 //   // first, measure. and total size.
 //   Size childrenSize;
 //   for (int iChild = 0; iChild < children->size(); iChild++) {
 //       auto& child = children->at(iChild);
 //       child->measureLayout(layoutContext, constraint);
 //       const Size& childDesiredSize = child->desiredSize();

 //       int cellIndex = getGridLayoutInfo()->layoutRow;
 //       if (cellIndex < 0) cellIndex = iChild;

	//	CellDefinition& cell = m_cellDefinitions[cellIndex];

 //       // セルサイズを子要素のサイズに合わせる場合
 //       if (cell.type == UILayoutLengthType::Auto) {
 //           if (Math::isNaN(cell.desiredSize)) cell.desiredSize = 0.0f; // initial
 //           if (isHorizontal())
 //               cell.desiredSize = std::max(cell.desiredSize, childDesiredSize.width);
 //           else
 //               cell.desiredSize = std::max(cell.desiredSize, childDesiredSize.height);
 //       }

 //       // total size
	//	if (isHorizontal()) {
	//		childrenSize.width += childDesiredSize.width;
	//		childrenSize.height = std::max(childrenSize.height, childDesiredSize.height);
	//	}
	//	else {
	//		childrenSize.width = std::max(childrenSize.width, childDesiredSize.width);
	//		childrenSize.height += childDesiredSize.height;
	//	}
 //   }

	//for (auto& thumb : m_thumbs) {
	//	thumb->measureLayout(layoutContext, constraint);
	//}

	//// bar area
	//if (isHorizontal()) {
	//	for (auto& thumb : m_thumbs) {
	//		childrenSize.width += thumb->desiredSize().width;
	//	}
	//}
	//else {
	//	for (auto& thumb : m_thumbs) {
	//		childrenSize.height += thumb->desiredSize().height;
	//	}
	//}

 //   // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
 //   Size selfSize = UIElement::measureOverride(layoutContext, constraint);
 //   return Size::max(selfSize, childrenSize);
}

Size UISplitter::arrangeOverride(UILayoutContext* layoutContext, const Rect& finalArea) {
    LN_NOTIMPLEMENTED();
    return Size();
	//const auto finalSize = finalArea.getSize();

	//if (m_lastArrangeFinalSize != finalSize) {
	//	// 最後に actualSize を決定した時と比べてサイズが変わっていたら再計算されるようにする
	//	for (int i = 0; i < m_cellDefinitions.size(); i++) {
	//		m_cellDefinitions[i].actualSize = Math::NaN;
	//	}
	//	m_lastArrangeFinalSize = finalSize;
	//}


 //   float boundSize = 0.0f;
 //   if (isHorizontal())
 //       boundSize = finalSize.width;
 //   else
 //       boundSize = finalSize.height;

	//float barTotalSize = 0.0f;
	//for (auto& thumb : m_thumbs) {
	//	if (isHorizontal()) {
	//		barTotalSize += thumb->desiredSize().width;
	//	}
	//	else {
	//		barTotalSize += thumb->desiredSize().height;
	//	}
	//}
	//boundSize -= barTotalSize;

	//// Fix size of 'Auto' and 'Direct', and count 'Ratio'
 //   float totalActualSize = 0.0f;
 //   float ratioCellCount = 0.0f;
 //   for (auto& cell : m_cellDefinitions) {
	//	if (cell.type == UILayoutLengthType::Ratio) {
	//		ratioCellCount += cell.size;
	//	}
	//	else {
	//		if (cell.type == UILayoutLengthType::Auto) {
	//			// measure で計算した desiredSize をそのまま使う
	//		}
	//		else if (cell.type == UILayoutLengthType::Direct) {
	//			cell.desiredSize = cell.size;
	//		}
	//		totalActualSize += cell.desiredSize;
	//	}
 //   }

 //   // "1*" 分のセルの領域を計算する
 //   float ratioUnit = (ratioCellCount != 0.0f) ? (boundSize - totalActualSize) / ratioCellCount : 0.0f;
 //   ratioUnit = std::max(0.0f, ratioUnit);	// 負値はダメ

 //   // "*" 指定である Row/Column の最終サイズを確定させ、
 //   // 全セルのオフセット (位置) も確定させる
 //   float totalOffset = 0.0f;
 //   //for (auto& cell : m_cellDefinitions) {
 //   for (int i = 0; i < m_cellDefinitions.size(); i++) {
 //       auto& cell = m_cellDefinitions[i];
 //       if (cell.type == UILayoutLengthType::Ratio) {
 //           cell.desiredSize = ratioUnit * cell.size;
 //       }

	//	// initial
	//	if (Math::isNaN(cell.actualSize)) {
	//		cell.actualSize = Math::clamp(cell.desiredSize, cell.minSize, cell.maxSize);
	//	}

 //       // fix offset
	//	cell.actualOffset = totalOffset;
 //       totalOffset += cell.actualSize;

 //       // calculate next offset
	//	if (i < m_thumbs.size()) {
	//		if (isHorizontal())
	//			totalOffset += m_thumbs[i]->desiredSize().width;
	//		else
	//			totalOffset += m_thumbs[i]->desiredSize().height;
	//	}
 //   }

 //   // 子要素の最終位置・サイズを確定させる
 //   auto& children = logicalChildren();
 //   if (isHorizontal()) {
 //       for (int iChild = 0; iChild < children->size(); iChild++) {
 //           auto& child = children->at(iChild);
 //           int cellIndex = child->getGridLayoutInfo()->layoutRow;
 //           if (cellIndex < 0) cellIndex = iChild;
 //           auto& cell = m_cellDefinitions[cellIndex];
 //           Rect childRect(cell.actualOffset, 0, cell.actualSize, finalSize.height);
 //           child->arrangeLayout(layoutContext, childRect);
 //       }
 //   }
 //   else {
 //       for (int iChild = 0; iChild < children->size(); iChild++) {
	//		auto& child = children->at(iChild);
 //           int cellIndex = child->getGridLayoutInfo()->layoutRow;
 //           if (cellIndex < 0) cellIndex = iChild;
 //           auto& cell = m_cellDefinitions[cellIndex];
 //           Rect childRect(0, cell.actualOffset, finalSize.width, cell.actualSize);
 //           child->arrangeLayout(layoutContext, childRect);
 //       }
 //   }

	//for (int iThumb = 0; iThumb < m_thumbs.size(); iThumb++) {
	//	auto& thumb = m_thumbs[iThumb];
	//	auto& cell = m_cellDefinitions[iThumb];
	//	Rect rect;
	//	if (isHorizontal()) {
	//		rect = Rect(cell.actualOffset + cell.actualSize, 0, thumb->desiredSize().width, finalSize.height);
	//	}
	//	else {
	//		rect = Rect(0, cell.actualOffset + cell.actualSize, finalSize.width, thumb->desiredSize().height);
	//	}
	//	thumb->arrangeLayout(layoutContext, rect);
	//}

 //   // 子要素のレイアウトは UIControl に任せず自分でやるので不要。そのベースを呼ぶ。
 //   Size selfSize = UIElement::arrangeOverride(layoutContext, finalArea);
 //   return selfSize;
}

void UISplitter::onRoutedEvent(UIEventArgs* e)
{
	/*if (e->type() == UIEvents::MouseDownEvent ||
		e->type() == UIEvents::MouseUpEvent ||
		e->type() == UIEvents::MouseMoveEvent)
	{
		m_thumbs[0]->raiseEvent(e, UIEventRoutingStrategy::Direct);
		e->handled = true;
	}
    else */if (e->type() == UIEvents::ScrollDragStartedEvent)
    {
		int index = m_thumbs.indexOf(static_cast<UIThumb*>(e->sender()));
		if (index >= 0) {
			auto& prev = m_cellDefinitions[index];
			auto& next = m_cellDefinitions[index + 1];
			m_dragStartSize1 = prev.actualSize;
			m_dragStartSize2 = next.actualSize;
		}

        e->handled = true;
    }
    else if (e->type() == UIEvents::ScrollDragDeltaEvent)
    {
		auto* e2 = static_cast<UIDragDeltaEventArgs*>(e);

		int index = m_thumbs.indexOf(static_cast<UIThumb*>(e->sender()));
		if (index >= 0) {
			auto& prev = m_cellDefinitions[index];
			auto& next = m_cellDefinitions[index + 1];

			float offset = isHorizontal() ? e2->offsetX() : e2->offsetY();

			if (m_dragStartSize1 + offset < 0) offset = -m_dragStartSize1;
			if (m_dragStartSize2 - offset < 0) offset = m_dragStartSize2;

			prev.actualSize = m_dragStartSize1 + offset;
			next.actualSize = m_dragStartSize2 - offset;

			invalidate(detail::UIElementDirtyFlags::Layout, true);
		}

        e->handled = true;
    }
    else if (e->type() == UIEvents::ScrollDragCompletedEvent)
    {
        e->handled = true;
    }
    UIElement::onRoutedEvent(e);
}

bool UISplitter::findNearThumb(const Point& pos, UIThumb** outThumb, float* outDistance)
{
	*outThumb = nullptr;
	*outDistance = std::numeric_limits<float>::max();

	const auto normal = isHorizontal() ? Vector3::UnitX : Vector3::UnitY;

	for (const auto& thumb : m_thumbs) {
		Plane plane(Vector3(thumb->actualPosition(), 0.0f), normal);
		const float d = std::abs(plane.getDistanceToPoint(Vector3(pos, 0.0f)));
		if (d < *outDistance) {
			*outThumb = thumb;
			*outDistance = d;
		}
	}

	return *outThumb != nullptr;
}

} // namespace ln


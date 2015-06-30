
/*
	DecreaseButton と IncreaseButton は WPF では
	<Track>
		<Track.DecreaseButton>
			<RepertButton />
		</Track.DecreaseButton>
		<Track.IncreaseButton>
			<RepertButton />
		</Track.IncreaseButton>
	</Track>
	のようにプロパティに直接入れている。

	実装が難しければ
	PART_DecreaseButton のような名前をつけて検索しても良いかもしれない。
*/

#include "../../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/ControlTemplate.h>
#include <Lumino/GUI/Controls/Track.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// Track
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Track);
LN_UI_ELEMENT_SUBCLASS_IMPL(Track);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Track::Track(GUIManager* manager)
	: UIElement(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Track::~Track()
{
}

} // namespace GUI
} // namespace Lumino


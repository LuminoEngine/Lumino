
#include "Internal.hpp"
#include "UIEditableTextArea.hpp"
#include "UIManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// TextRange

void TextRange::splitLineRanges(const String& str, List<TextRange>* outRanges)
{
	int lineBeginIndex = 0;

	const Char* begin = str.c_str();
	for (const Char* ch = begin; ch && *ch; ++ch)
	{
		bool isCRLF = (*ch == '\r' && *(ch + 1) == '\n');
		if (*ch == '\r' || *ch == '\n')
		{
			int lineEndIndex = (ch - begin);
			assert(lineEndIndex >= lineBeginIndex);
			outRanges->add(TextRange{ lineBeginIndex, lineEndIndex });

			if(isCRLF) {
				++ch;
			}
			lineBeginIndex = (ch - begin) + 1;
		}
	}

	if (lineBeginIndex <= str.length()) {
		outRanges->add(TextRange{ lineBeginIndex, str.length() });
	}
}

//==============================================================================
// UIEditableTextArea

UIEditableTextArea::UIEditableTextArea()
{
}

} // namespace detail
} // namespace ln


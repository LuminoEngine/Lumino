#pragma once

namespace ln {
namespace detail {

struct TextLocation
{
    int lineIndex = 0;  // 0 start
    int offset = 0;     // offset from line head (unit: Char. 2 point for surrogates)
};

// A, B の前後関係は問わない。
struct TextSelection
{
    TextLocation locationA;
    TextLocation locationB;

    const TextSelection& getBegin() const
	{
		if (locationA.lineIndex == locationB.lineIndex) {
			if (locationA.offset < locationB.offset) {
				return locationA;
			}
			return locationB;
		}
		else if (locationA.lineIndex < locationB.lineIndex) {
			return locationA;
		}

		return locationB;
	}

	const TextSelection& getEnd() const
	{
		if (locationA.lineIndex == locationB.lineIndex) {
			if (locationA.offset > locationB.offset) {
				return locationA;
			}
			return locationB;
		}
		else if (locationA.lineIndex > locationB.lineIndex) {
			return locationA;
		}
		return locationB;
	}
};

struct TextRange
{
    static void splitLineRanges(const String& str, List<TextRange>* outRangess);
    
	int beginIndex = 0; // (unit: Char)
	int endIndex = 0;   // (unit: Char)
};

class LineModel
    : public RefObject
{
public:

private:
    String  m_text;
};

class RunModel
    : public RefObject
{
public:

private:
    LineModel* m_ownerLineModel;
    TextRange m_range;  // ranfe of m_ownerLineModel->m_text
};

class TextArea
    : public RefObject
{
public:

private:
};

class UIEditableTextArea
    : public UIElement
{
};

} // namespace detail
} // namespace ln


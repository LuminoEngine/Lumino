#pragma once
#include <LuminoGraphics/Rendering/CommandList.hpp>
#include "RenderElement.hpp"

namespace ln {
namespace detail {
class RenderDrawElement;

class CommandListServer
	: public RefObject
{
public:
	struct Part
	{
		Ref<CommandList> primaryList;
	};

	CommandList* acquirePrimaryList(RenderPart index1, const RenderViewPoint* viewPoint);
	CommandList* getPrimaryList(RenderPart index1) const;
	void clearCommandsAndState(const RenderViewPoint* viewPoint);



	template<class TCallback>
	inline void enumerateDrawElements(RenderPart index1, TCallback callback) const
	{
		const Part& part = m_parts[static_cast<int>(index1)];
		if (part.primaryList) {
			const auto& list = part.primaryList->elementList();
			RenderDrawElement* element = list->headElement();
			while (element) {
				callback(element);
				//element = element->m_classifiedNext;
				element = element->m_next;
			}
		}
	}

private:
	std::array<Part, 4> m_parts;
};

} // namespace detail
} // namespace ln


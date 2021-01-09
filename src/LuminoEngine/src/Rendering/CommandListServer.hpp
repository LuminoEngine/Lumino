#pragma once
#include <LuminoEngine/Rendering/CommandList.hpp>
#include "RenderElement.hpp"

namespace ln {
namespace detail {
class RenderDrawElement;

class CommandListServer
	: public RefObject
{
public:
	struct Layer
	{
		Ref<CommandList> primaryList;
		//std::vector<Ref<DrawElementList>> additionalList;
	};

	struct Part
	{
		std::array<Layer, 4> layers;
	};

	CommandList* acquirePrimaryList(RenderPart index1, ProjectionKind index2);
	CommandList* getPrimaryList(RenderPart index1, ProjectionKind index2) const;
	void clearCommandsAndState();


	const Layer* getLayer(RenderPart index1, ProjectionKind index2) const;

	template<class TCallback>
	inline void enumerateDrawElements(RenderPart index1, ProjectionKind index2, TCallback callback) const
	{
		const Part& part = m_parts[static_cast<int>(index1)];
		const Layer& layer = part.layers[static_cast<int>(index2)];
		if (layer.primaryList) {
			const auto& list = layer.primaryList->elementList();
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


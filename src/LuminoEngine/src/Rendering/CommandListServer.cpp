
#include "Internal.hpp"
#include "CommandListServer.hpp"

namespace ln {
namespace detail {
    
//==============================================================================
// CommandListServer

CommandList* CommandListServer::acquirePrimaryList(RenderPart index1, ProjectionKind index2)
{
	Part& part = m_parts[static_cast<int>(index1)];
	Layer& layer = part.layers[static_cast<int>(index2)];
	if (!layer.primaryList) {
		layer.primaryList = makeObject<CommandList>();

		// 作成済みの CommandList の clearCommandsAndState() は描画開始時に呼ばれるが、
		// 描画中に作られた CommandList のインスタンスに対しては個別に呼び出す必要がある。
		layer.primaryList->clearCommandsAndState();
	}
	return layer.primaryList;
}

CommandList* CommandListServer::getPrimaryList(RenderPart index1, ProjectionKind index2) const
{
	const Part& part = m_parts[static_cast<int>(index1)];
	const Layer& layer = part.layers[static_cast<int>(index2)];
	return layer.primaryList;
}

void CommandListServer::clearCommandsAndState()
{
	for (auto& part : m_parts) {
		for (auto& layer : part.layers) {
			if (layer.primaryList) {
				layer.primaryList->clearCommandsAndState();
			}
		}
	}
}

const CommandListServer::Layer* CommandListServer::getLayer(RenderPart index1, ProjectionKind index2) const
{
	const Part& part = m_parts[static_cast<int>(index1)];
	const Layer& layer = part.layers[static_cast<int>(index2)];
	return &layer;
}

} // namespace detail
} // namespace ln


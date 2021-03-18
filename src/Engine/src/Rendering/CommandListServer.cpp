
#include "Internal.hpp"
#include "CommandListServer.hpp"

namespace ln {
namespace detail {
    
//==============================================================================
// CommandListServer

CommandList* CommandListServer::acquirePrimaryList(RenderPart index1)
{
	Part& part = m_parts[static_cast<int>(index1)];
	if (!part.primaryList) {
		part.primaryList = makeObject<CommandList>();

		// 作成済みの CommandList の clearCommandsAndState() は描画開始時に呼ばれるが、
		// 描画中に作られた CommandList のインスタンスに対しては個別に呼び出す必要がある。
		part.primaryList->clearCommandsAndState();
	}
	return part.primaryList;
}

CommandList* CommandListServer::getPrimaryList(RenderPart index1) const
{
	const Part& part = m_parts[static_cast<int>(index1)];
	return part.primaryList;
}

void CommandListServer::clearCommandsAndState()
{
	for (auto& part : m_parts) {
		if (part.primaryList) {
			part.primaryList->clearCommandsAndState();
		}
	}
}

} // namespace detail
} // namespace ln


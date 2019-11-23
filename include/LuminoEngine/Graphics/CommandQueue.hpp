// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

namespace ln {
namespace detail {
class GraphicsManager;
class ICommandQueue;
}

/**  */
class CommandQueue
    : public Object
{
public:

private:
	CommandQueue();
	void init(detail::ICommandQueue* rhiObject);

    Ref<detail::ICommandQueue> m_rhiObject;

	friend class detail::GraphicsManager;
};

} // namespace ln

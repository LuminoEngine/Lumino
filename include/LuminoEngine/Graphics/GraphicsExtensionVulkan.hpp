// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "GraphicsExtension.hpp"
#include <vulkan/vulkan.h>

namespace ln {

class IVulkanNativeGraphicsInterface : public INativeGraphicsInterface
{
public:
	virtual VkInstance getInstance() const = 0;
	virtual VkPhysicalDevice getPhysicalDevice() const = 0;
	virtual VkDevice getDevice() const = 0;
	virtual VkQueue getGraphicsQueue() const = 0;	// レンダリングに使用している GraphicsQueue
	virtual uint32_t getGraphicsQueueFamilyIndex() const = 0;

    // RecordingState
    virtual VkCommandBuffer getRecordingCommandBuffer() const = 0;
};

} // namespace ln

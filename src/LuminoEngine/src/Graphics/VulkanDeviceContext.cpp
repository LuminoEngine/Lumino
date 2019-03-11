
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/PlatformSupport.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../Platform/GLFWPlatformWindowManager.hpp"

#include "VulkanDeviceContext.hpp"

//#define STB_IMAGE_IMPLEMENTATION
#include "../../../../build/ExternalSource/stb/stb_image.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <array>
#include <optional>
#include <set>

namespace ln {
namespace detail {

const int WIDTH = 800;
const int HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;


const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct Vertex {
    Vector3 pos;
	Vector3 color;
	Vector2 texCoord;

    //static VkVertexInputBindingDescription getBindingDescription() {
    //    VkVertexInputBindingDescription bindingDescription = {};
    //    bindingDescription.binding = 0;
    //    bindingDescription.stride = sizeof(Vertex);
    //    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    //    return bindingDescription;
    //}

    //static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
    //    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

    //    attributeDescriptions[0].binding = 0;
    //    attributeDescriptions[0].location = 0;
    //    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    //    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    //    attributeDescriptions[1].binding = 0;
    //    attributeDescriptions[1].location = 1;
    //    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    //    attributeDescriptions[1].offset = offsetof(Vertex, color);

    //    attributeDescriptions[2].binding = 0;
    //    attributeDescriptions[2].location = 2;
    //    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    //    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    //    return attributeDescriptions;
    //}
};

struct UniformBufferObject {
    alignas(16) Matrix model;
    alignas(16) Matrix view;
    alignas(16) Matrix proj;
};

std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
};



class HelloTriangleApplication {
public:
    //void run() {
    //    initWindow();
    //    initVulkan();
    //    mainLoop();
    //    cleanup();
    //}

public:
    VulkanDeviceContext* m_deviceContext;

    GLFWwindow* window;

    VkSurfaceKHR surface;

    VkDevice device;

    //VkQueue graphicsQueue;
    //VkQueue presentQueue;

    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    //std::vector<VkImage> swapChainImages;
    //std::vector<VkImageView> swapChainImageViews;
    //Ref<VulkanSwapchainRenderTargetTexture> m_swapchainRenderTarget;
	std::vector<Ref<VulkanRenderTarget>> m_swapchainRenderTargets;

    Ref<VulkanShaderPass> m_shaderPass;
        
    //VkRenderPass renderPass;
    //VkPipelineLayout pipelineLayout;
    //VkPipeline graphicsPipeline;
    //Ref<VulkanPipeline> m_graphicsPipeline;

    Ref<VulkanDepthBuffer> m_depthImage;

    Ref<VulkanTexture2D> m_texture;
    Ref<VulkanSamplerState> m_textureSampler;
    //VkSampler textureSampler;

    Ref<VulkanVertexDeclaration> m_vertexDeclaration;
    Ref<VulkanVertexBuffer> m_vertexBuffer;
    Ref<VulkanIndexBuffer> m_indexBuffer;

    VulkanBuffer m_uniformBuffer;
    //std::vector<std::shared_ptr<VulkanBuffer>> m_uniformBuffers;
    //std::vector<VkBuffer> uniformBuffers;
    //std::vector<VkDeviceMemory> uniformBuffersMemory;

    std::vector<Ref<VulkanCommandBuffer>> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    size_t currentFrame = 0;

    bool framebufferResized = false;

    void initWindow(GLFWwindow* glfwwindow, int width, int height) {
        //glfwInit();

        //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwwindow;// glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		framebufferResized = true;
        //glfwSetWindowUserPointer(window, this);
        //glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

		//framebufferResizeCallback(glfwwindow, width, height);
    }

    //static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    //    auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    //    app->framebufferResized = true;
    //}

    void initVulkan() {


        VertexElement elements[] = {
            { 0, VertexElementType::Float3, VertexElementUsage::Position, 0 },
            { 0, VertexElementType::Float3, VertexElementUsage::Color, 0 },
            { 0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0 },
        };
        m_vertexDeclaration = makeRef<VulkanVertexDeclaration>();
        m_vertexDeclaration->init(elements, 3);

        //createInstance();
        //setupDebugMessenger();
        //createSurface();
        //pickPhysicalDevice();   m_deviceContext->m_physicalDevice = physicalDevice;
        //createLogicalDevice();  m_deviceContext->m_device = device;
        createSwapChain();
        createDepthResources();
        //createImageViews();
        //createDescriptorSetLayout();
        createGraphicsPipeline();
        //createCommandPool();
        //createFramebuffers();
        createTextureImage();
        //createTextureImageView();
        createTextureSampler();
        createVertexBuffer();
        createIndexBuffer();
        createUniformBuffers();
        //createDescriptorPool();
        createCommandBuffers();
        createSyncObjects();
    }

    void mainLoop() {
        //while (!glfwWindowShouldClose(window)) {
        //    glfwPollEvents();
            drawFrame();
        //}

        vkDeviceWaitIdle(device);
    }

    void cleanupSwapChain() {

		m_depthImage->dispose();

        for (auto c : commandBuffers) {
            c->dispose();
        }
        commandBuffers.clear();

        m_shaderPass->dispose();

        //vkDestroyPipeline(device, graphicsPipeline, nullptr);
        //vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
        //m_graphicsPipeline->dispose();
        //m_graphicsPipeline = nullptr;

        //for (auto imageView : swapChainImageViews) {
        //    vkDestroyImageView(device, imageView, nullptr);
        //}
		//m_swapchainRenderTarget->dispose();
        //m_swapchainRenderTarget = nullptr;
		for (auto& rt : m_swapchainRenderTargets) {
			rt->dispose();
		}
		m_swapchainRenderTargets.clear();

        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }

    void cleanup() {
        cleanupSwapChain();

        m_textureSampler->dispose();

		m_texture->dispose();

        m_uniformBuffer.dispose();
        //for (auto& buf : m_uniformBuffers) {
        //    buf->dispose();
        //}

        m_indexBuffer->dispose();
        m_vertexBuffer->dispose();

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }

    }

    void recreateSwapChain() {
        int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(device);

        cleanupSwapChain();

        createSwapChain();
        createDepthResources();
        //createImageViews();
        createGraphicsPipeline();
        //createFramebuffers();
        createCommandBuffers();
    }

    void createSwapChain() {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_deviceContext->vulkanPhysicalDevice());

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = m_deviceContext->findQueueFamilies(m_deviceContext->vulkanPhysicalDevice());
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }



        std::vector<VkImage> swapChainImages;
        {
            vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
            swapChainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

            swapChainImageFormat = surfaceFormat.format;
            swapChainExtent = extent;
        }

        std::vector<VkImageView> swapChainImageViews;
        {
            swapChainImageViews.resize(swapChainImages.size());

            for (uint32_t i = 0; i < swapChainImages.size(); i++) {
                swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
            }
        }

		m_swapchainRenderTargets.resize(swapChainImages.size());
		for (uint32_t i = 0; i < swapChainImages.size(); i++) {
			auto target = makeRef<VulkanRenderTarget>();
			target->init(m_deviceContext, swapChainExtent.width, swapChainExtent.height, swapChainImageFormat, swapChainImages[i], swapChainImageViews[i]);
            m_swapchainRenderTargets[i] = target;
		}
        //m_swapchainRenderTarget = makeRef<VulkanSwapchainRenderTargetTexture>();
        //m_swapchainRenderTarget->init(m_deviceContext);
        //m_swapchainRenderTarget->reset(swapChainExtent.width, swapChainExtent.height, swapChainImageFormat, swapChainImages, swapChainImageViews);
    }

    void createGraphicsPipeline() {
        m_deviceContext->refreshCaps();
        auto diag = newObject<DiagnosticsManager>();
        auto stream = FileStream::create("D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Graphics/Resource/VulkanSampleDeviceContext_26_shader_depth.lcfx");
        detail::UnifiedShader unifiedShader(diag);
        unifiedShader.load(stream);
        m_shaderPass = ln::dynamic_pointer_cast<VulkanShaderPass>(m_deviceContext->createShaderPassFromUnifiedShaderPass(&unifiedShader, 1, diag));


        //m_graphicsPipeline = makeRef<VulkanPipeline>();
        //m_graphicsPipeline->init(m_deviceContext, state);
    }

    // fix
    void createVertexBuffer() {

        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        m_vertexBuffer = makeRef<VulkanVertexBuffer>();
        m_vertexBuffer->init(m_deviceContext, GraphicsResourceUsage::Static, bufferSize, vertices.data());
    }

    // fix
    void createIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        m_indexBuffer = makeRef<VulkanIndexBuffer>();
        m_indexBuffer->init(m_deviceContext, GraphicsResourceUsage::Static, IndexBufferFormat::UInt16, indices.size(), indices.data());
    }

    // fix
    void createDepthResources() {
        m_depthImage = makeRef<VulkanDepthBuffer>();
        m_depthImage->init(m_deviceContext, swapChainExtent.width, swapChainExtent.height);
    }

    // fix
    void createTextureImage() {
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load("D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/src/Graphics/Resource/texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels) {
            throw std::runtime_error("failed to load texture image!");
        }

        m_texture = makeRef<VulkanTexture2D>();
        m_texture->init(m_deviceContext, texWidth, texHeight, TextureFormat::RGBA32, false, pixels);

        stbi_image_free(pixels);
    }

    void createTextureSampler() {

        SamplerStateData desc;
        desc.filter = TextureFilterMode::Linear;
        desc.address = TextureAddressMode::Repeat;
        m_textureSampler = makeRef<VulkanSamplerState>();
        m_textureSampler->init(m_deviceContext, desc);
    }

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }

    void createUniformBuffers() {
        m_uniformBuffer.init(m_deviceContext, sizeof(UniformBufferObject), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    void createCommandBuffers() {
        commandBuffers.resize(m_swapchainRenderTargets.size());

        for (size_t i = 0; i < commandBuffers.size(); i++) {
            commandBuffers[i] = makeRef<VulkanCommandBuffer>();
            commandBuffers[i]->init(m_deviceContext);

        }
    }

    void createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }
    }

    void updateFrameData(uint32_t imageIndex, VulkanCommandBuffer* commandBuffer) {

        m_shaderPass->samplerBuffer()->setTexture(0, m_texture);
        m_shaderPass->samplerBuffer()->setSamplerState(1, m_textureSampler);


        DeviceFramebufferState framebufferState;
        framebufferState.renderTargets[0] = m_swapchainRenderTargets[imageIndex];
        framebufferState.depthBuffer = m_depthImage;
        VulkanFramebuffer* framebuffer = m_deviceContext->framebufferCache()->findOrCreate(framebufferState);


        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo = {};
		ubo.model = Matrix::makeRotationY(time * 0.01);  //glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = Matrix::makeLookAtLH(Vector3(2.0f, 2.0f, 2.0f), Vector3::Zero, Vector3::UnitZ);//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = Matrix::makePerspectiveFovLH(0.3, swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);// glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
        //ubo.proj[1][1] *= -1;
        // TODO: 転置
        // HlslParseContext::setUniformBlockDefaults で、uniform block のデフォルトの行列レイアウトを ElmRowMajor にしているため、
        // 例えば GLSL コードにすると
        //   layout(std140) uniform _Global {
        //     layout(row_major) mat4 model;
        //     layout(row_major) mat4 view;
        //     layout(row_major) mat4 proj;
        //   };
        // のように出力される。
        // 外部からこれを制御する方法はなさそうなので、使う側で転置してカバーする。
        // ちなみに、ElmColumnMajor にすると上の layout(row_major) の部分は消える（GLSL に出力されない）
        ubo.model.transpose();
        ubo.view.transpose();
        ubo.proj.transpose();
        ////ubo.model = ubo.model * ubo.view * ubo.proj;
        //ubo.model.transpose();

        commandBuffer->beginRecording();

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = framebuffer->vulkanRenderPass();//renderPass;
        renderPassInfo.framebuffer = framebuffer->vulkanFramebuffer();
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChainExtent;

        std::array<VkClearValue, 2> clearValues = {};
        clearValues[0].color = { 0.0f, 0.0f, 1.0f, 1.0f };
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer->vulkanCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);



        IGraphicsDeviceContext::State state;
        state.pipelineState.shaderPass = m_shaderPass;
        state.pipelineState.vertexDeclaration = m_vertexDeclaration;
        state.framebufferState.renderTargets[0] = m_swapchainRenderTargets[0];
        state.framebufferState.depthBuffer = m_depthImage;
        state.viewportRect.width = swapChainExtent.width;
        state.viewportRect.height = swapChainExtent.height;
        VulkanPipeline* graphicsPipeline = m_deviceContext->pipelineCache()->findOrCreate(state, framebuffer->vulkanRenderPass());


        vkCmdBindPipeline(commandBuffer->vulkanCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->vulkanPipeline());//graphicsPipeline);

        VkBuffer vertexBuffers[] = { m_vertexBuffer->vulkanBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer->vulkanCommandBuffer(), 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffer->vulkanCommandBuffer(), m_indexBuffer->vulkanBuffer(), 0, m_indexBuffer->indexType());

        // UniformBuffer は copy コマンドを使って更新できる。
        // TODO: ただし、texture や sampler は vkUpdateDescriptorSets でしか更新できないのでこれもキャッシュしたりする仕組みがほしいところ。
        //VulkanBuffer* buffer = commandBuffer->cmdCopyBuffer(sizeof(ubo), &m_uniformBuffer);
        VulkanShaderUniformBuffer* uniformBuffer = static_cast<VulkanShaderUniformBuffer*>(m_shaderPass->getUniformBuffer(0));
        uniformBuffer->setData(&ubo, sizeof(ubo));


        std::array<VkDescriptorSet, DescriptorType_Count> sets;
        commandBuffer->allocateDescriptorSets(m_shaderPass, &sets);
        vkCmdBindDescriptorSets(commandBuffer->vulkanCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_shaderPass->vulkanPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);

        //
        // test
        //vertices[0].pos.x = 0;
        //vertices[0].pos.y = 0;
        //VulkanBuffer* buffer = commandBuffer->cmdCopyBuffer(sizeof(vertices[0]) * vertices.size(), m_vertexBuffer->buffer());
        //buffer->setData(0, vertices.data(), sizeof(vertices[0]) * vertices.size());

//#if 1
//        m_uniformBuffers[imageIndex]->setData(0, &ubo, sizeof(ubo));
//#else
//        // CombindSamper ではなく、個別設定のレイアウトを使っていると、実行中キューと記録中キューの間で共有できないようだ。
//        // SubmitQueue が失敗する。
//        VulkanBuffer* buffer = commandBuffer->cmdCopyBuffer(sizeof(ubo), m_uniformBuffers[imageIndex].get());//&m_uniformBuffer);
//        buffer->setData(0, &ubo, sizeof(ubo));
//#endif

        vkCmdDrawIndexed(commandBuffer->vulkanCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        // test
        //vertices[0].pos.x = 0;
        //vertices[0].pos.y = 0;
        //m_vertexBuffer->setSubData(0, vertices.data(), sizeof(vertices[0]) * vertices.size());

        vkCmdEndRenderPass(commandBuffer->vulkanCommandBuffer());

        commandBuffer->endRecording();

        //m_uniformBuffers[currentImage].setData(0, &ubo, sizeof(ubo));
    }

    void drawFrame() {
        vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device, swapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        //m_swapchainRenderTarget->setCurrentBufferIndex(imageIndex);

        updateFrameData(imageIndex, commandBuffers[imageIndex]);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        VkCommandBuffer commandBuffer = commandBuffers[imageIndex]->vulkanCommandBuffer();
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(device, 1, &inFlightFences[currentFrame]);

        if (vkQueueSubmit(m_deviceContext->m_graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        result = vkQueuePresentKHR(m_deviceContext->m_presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
            framebufferResized = false;
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
            return{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        }

        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
        VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                bestMode = availablePresentMode;
            }
        }

        return bestMode;
    }

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
};























//==============================================================================


HelloTriangleApplication g_app;


VulkanDeviceContext::VulkanDeviceContext()
{
}

bool VulkanDeviceContext::init(const Settings& settings)
{
	GLFWPlatformWindow* glfwWindow = dynamic_cast<GLFWPlatformWindow*>(settings.mainWindow);
	SizeI size;
	glfwWindow->getSize(&size);
    g_app.m_deviceContext = this;
	m_mainWindow = glfwWindow->glfwWindow();

    if (!createInstance()) {
        return false;
    }
    if (!setupDebugMessenger()) {
        return false;
    }
	if (!createSurface()) {
		return false;
	}
    if (!pickPhysicalDevice()) {
        return false;
    }
	if (!createLogicalDevice()) {
		return false;
	}
    if (!createCommandPool()) {
        return false;
    }

    if (!m_renderPassCache.init(this)) {
        return false;
    }
    if (!m_framebufferCache.init(this)) {
        return false;
    }
    if (!m_pipelineCache.init(this)) {
        return false;
    }

	g_app.surface = m_mainSurface;
	g_app.device = m_device;
	g_app.initWindow(glfwWindow->glfwWindow(), size.width, size.height);
	g_app.initVulkan();
	return true;
}

void VulkanDeviceContext::dispose()
{
    g_app.cleanup();

    m_pipelineCache.dispose();
    m_framebufferCache.dispose();
    m_renderPassCache.dispose();

    if (m_commandPool) {
        vkDestroyCommandPool(m_device, m_commandPool, vulkanAllocator());
        m_commandPool = VK_NULL_HANDLE;
    }

	if (m_device) {
		vkDestroyDevice(m_device, vulkanAllocator());
		m_device = VK_NULL_HANDLE;
	}

	if (m_mainSurface) {
		vkDestroySurfaceKHR(vulkanInstance(), m_mainSurface, vulkanAllocator());
		m_mainSurface = VK_NULL_HANDLE;
	}

    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(vulkanInstance(), m_debugMessenger, vulkanAllocator());
    }

    if (m_instance) {
        vkDestroyInstance(m_instance, vulkanAllocator());
        m_instance = VK_NULL_HANDLE;
    }
}

void VulkanDeviceContext::onGetCaps(GraphicsDeviceCaps * outCaps)
{
    outCaps->requestedShaderTriple.target = "spv";
    outCaps->requestedShaderTriple.version = 110;
    outCaps->requestedShaderTriple.option = "";
}

void VulkanDeviceContext::onEnterMainThread()
{
}

void VulkanDeviceContext::onLeaveMainThread() 
{
}

void VulkanDeviceContext::onSaveExternalRenderState()
{
}

void VulkanDeviceContext::onRestoreExternalRenderState()
{
}

Ref<ISwapChain> VulkanDeviceContext::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	auto ptr = makeRef<VulkanSwapChain>();
    if (!ptr->init(this, window, backbufferSize)) {
        return nullptr;
    }
	return ptr;
}

Ref<IVertexDeclaration> VulkanDeviceContext::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount)
{
	return nullptr;
}

Ref<IVertexBuffer> VulkanDeviceContext::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
	return nullptr;
}

Ref<IIndexBuffer> VulkanDeviceContext::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
	return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateTexture2D(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

Ref<ITexture> VulkanDeviceContext::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

Ref<IDepthBuffer> VulkanDeviceContext::onCreateDepthBuffer(uint32_t width, uint32_t height)
{
	auto ptr = makeRef<VulkanDepthBuffer>();
    if (!ptr->init(this, width, height)) {
        return nullptr;
    }
	return ptr;
}

Ref<ISamplerState> VulkanDeviceContext::onCreateSamplerState(const SamplerStateData& desc)
{
	auto ptr = makeRef<VulkanSamplerState>();
	if (!ptr->init(this, desc)) {
		return nullptr;
	}
	return ptr;
}

Ref<IShaderPass> VulkanDeviceContext::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    auto ptr = makeRef<VulkanShaderPass>();
    if (!ptr->init(this, createInfo, diag)) {
        return nullptr;
    }
    return ptr;
}

void VulkanDeviceContext::onUpdatePipelineState(const BlendStateDesc& blendState, const RasterizerStateDesc& rasterizerState, const DepthStencilStateDesc& depthStencilState)
{
}

void VulkanDeviceContext::onUpdateFrameBuffers(ITexture** renderTargets, int renderTargetsCount, IDepthBuffer* depthBuffer)
{
}

void VulkanDeviceContext::onUpdateRegionRects(const RectI& viewportRect, const RectI& scissorRect, const SizeI& targetSize)
{
}

void VulkanDeviceContext::onUpdatePrimitiveData(IVertexDeclaration* decls, IVertexBuffer** vertexBuufers, int vertexBuffersCount, IIndexBuffer* indexBuffer)
{
}

void VulkanDeviceContext::onUpdateShaderPass(IShaderPass* newPass)
{
}

void VulkanDeviceContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
}

void VulkanDeviceContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount)
{
}

void VulkanDeviceContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount)
{
}

void VulkanDeviceContext::onPresent(ISwapChain* swapChain)
{
	g_app.mainLoop();
}

Result VulkanDeviceContext::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, uint32_t* outType)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            *outType = i;
            return false;
        }
    }

    LN_LOG_ERROR << "failed to find suitable memory type!";
    return false;
}

static std::vector<const char*> getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

Result VulkanDeviceContext::createInstance()
{
    if (enableValidationLayers && !VulkanHelper::checkValidationLayerSupport()) {
        LN_LOG_ERROR << "validation layers requested, but not available!";
        return false;
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanHelper::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanHelper::validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    LN_VK_CHECK(vkCreateInstance(&createInfo, vulkanAllocator(), &m_instance));

    return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

Result VulkanDeviceContext::setupDebugMessenger()
{
	if (!enableValidationLayers) return false;

	VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;

	LN_VK_CHECK(CreateDebugUtilsMessengerEXT(vulkanInstance(), &createInfo, vulkanAllocator(), &m_debugMessenger));

	return true;
}

Result VulkanDeviceContext::createSurface()
{
	LN_VK_CHECK(glfwCreateWindowSurface(vulkanInstance(), m_mainWindow, vulkanAllocator(), &m_mainSurface));
	return true;
}

Result VulkanDeviceContext::pickPhysicalDevice()
{
    // Get physical devices
    {
        uint32_t count = 0;
        auto ret = vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
        if (ret != VK_SUCCESS || count < 1) {
            LN_LOG_ERROR << "Failed vkEnumeratePhysicalDevices";
            return false;
        }

        m_physicalDeviceInfos.resize(count);

        std::vector<VkPhysicalDevice> gpuDevices(count);
        if (vkEnumeratePhysicalDevices(m_instance, &count, gpuDevices.data()) != VK_SUCCESS) {
            LN_LOG_ERROR << "Failed vkEnumeratePhysicalDevices";
            return false;
        }

        for (auto i = 0u; i < count; ++i) {
            m_physicalDeviceInfos[i].device = gpuDevices[i];
            vkGetPhysicalDeviceMemoryProperties(gpuDevices[i], &m_physicalDeviceInfos[i].memoryProperty);
            vkGetPhysicalDeviceProperties(gpuDevices[i], &m_physicalDeviceInfos[i].deviceProperty);
        }
    }

    // Select device
    // TODO:
    m_physicalDevice = m_physicalDeviceInfos[0].device;

    return true;
}

Result VulkanDeviceContext::createLogicalDevice()
{
	QueueFamilyIndices indices = findQueueFamilies(vulkanPhysicalDevice());

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanHelper::validationLayers.size());
		createInfo.ppEnabledLayerNames = VulkanHelper::validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(vulkanPhysicalDevice(), &createInfo, nullptr, &m_device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);

	return true;
}

Result VulkanDeviceContext::createCommandPool()
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(vulkanPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    LN_VK_CHECK(vkCreateCommandPool(m_device, &poolInfo, vulkanAllocator(), &m_commandPool));

    return true;
}




QueueFamilyIndices VulkanDeviceContext::findQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_mainSurface, &presentSupport);

		if (queueFamily.queueCount > 0 && presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}

VkFormat VulkanDeviceContext::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(vulkanPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    LN_UNREACHABLE();
    return VK_FORMAT_UNDEFINED;
}

VkFormat VulkanDeviceContext::findDepthFormat()
{
    return findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkCommandBuffer VulkanDeviceContext::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vulkanCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanDeviceContext::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);

    vkFreeCommandBuffers(m_device, vulkanCommandPool(), 1, &commandBuffer);
}

void VulkanDeviceContext::copyBufferImmediately(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void VulkanDeviceContext::copyBufferToImageImmediately(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    endSingleTimeCommands(commandBuffer);
}

Result VulkanDeviceContext::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (VulkanHelper::hasStencilComponent(format)) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else {
        LN_LOG_ERROR << "unsupported layout transition!";
        return false;
    }

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    endSingleTimeCommands(commandBuffer);

    return true;
}

//==============================================================================
// VulkanSwapChain

VulkanSwapChain::VulkanSwapChain()
{
}

Result VulkanSwapChain::init(VulkanDeviceContext* deviceContext, PlatformWindow* window, const SizeI& backbufferSize)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

	m_colorBuffer = makeRef<VulkanRenderTarget>();
	return true;
}

ITexture* VulkanSwapChain::getColorBuffer() const
{
	return m_colorBuffer;
}

//==============================================================================
// VulkanVertexDeclaration

VulkanVertexDeclaration::VulkanVertexDeclaration()
{
}

Result VulkanVertexDeclaration::init(const VertexElement* elements, int elementsCount)
{
    LN_DCHECK(elements);

    // 事前に binding がどれだけあるのか調べる
    m_maxStreamCount = 0;
    for (int i = 0; i < elementsCount; i++) {
        m_maxStreamCount = std::max(m_maxStreamCount, elements[i].StreamIndex);
        m_elements.push_back(elements[i]);
    }
    m_maxStreamCount++;
    m_bindings.resize(m_maxStreamCount);

    // 実際に値を計算する
    for (int i = 0; i < elementsCount; i++) {
        auto& element = m_elements[i];

        AttributeDescriptionSource attr;
        attr.binding = elements[i].StreamIndex;
        attr.format = VulkanHelper::LNVertexElementTypeToVkFormat(elements[i].Type);
        attr.offset = m_bindings[attr.binding].stride;
        m_attributeSources.push_back(attr);

        m_bindings[element.StreamIndex].stride += GraphicsHelper::getVertexElementTypeSize(elements[i].Type);
    }

    m_hash = computeHash(m_elements);

    return true;
}

void VulkanVertexDeclaration::dispose()
{
    IVertexDeclaration::dispose();
}

uint64_t VulkanVertexDeclaration::computeHash(const std::vector<VertexElement>& elements)
{
    MixHash hash;
    hash.add(elements.size());
    for (auto& e : elements) {
        hash.add(e.StreamIndex);
        hash.add(e.Type);
        hash.add(e.Usage);
        hash.add(e.UsageIndex);
    }
    return hash.value();
}

//==============================================================================
// VulkanVertexBuffer

VulkanVertexBuffer::VulkanVertexBuffer()
    : m_buffer()
    , m_usage(GraphicsResourceUsage::Static)
{
}

Result VulkanVertexBuffer::init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    LN_DCHECK(deviceContext);

    m_usage = usage;

    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
        return false;
    }

    if (usage == GraphicsResourceUsage::Dynamic) {
        m_buffer.setData(0, initialData, bufferSize);
    }
    else {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            return false;
        }
        stagingBuffer.setData(0, initialData, bufferSize);
        m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.vulkanBuffer(), vulkanBuffer(), bufferSize);
        stagingBuffer.dispose();
    }

    return true;
}

void VulkanVertexBuffer::dispose()
{
    m_buffer.dispose();
}

void VulkanVertexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    // TODO: これは廃止して、CommandList 側に持って行った方がいいと思う。
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// VulkanIndexBuffer

VulkanIndexBuffer::VulkanIndexBuffer()
    : m_buffer()
    , m_usage(GraphicsResourceUsage::Static)
{
}

Result VulkanIndexBuffer::init(VulkanDeviceContext* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    LN_DCHECK(deviceContext);

    m_usage = usage;
    int stride = 0;
    if (format == IndexBufferFormat::UInt16) {
        m_indexType = VK_INDEX_TYPE_UINT16;
        stride = 2;
    }
    else {
        m_indexType = VK_INDEX_TYPE_UINT32;
        stride = 4;
    }
    size_t bufferSize = stride * indexCount;

    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
        return false;
    }

    if (usage == GraphicsResourceUsage::Dynamic) {
        m_buffer.setData(0, initialData, bufferSize);
    }
    else {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            return false;
        }
        stagingBuffer.setData(0, initialData, bufferSize);
        m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.vulkanBuffer(), vulkanBuffer(), bufferSize);
        stagingBuffer.dispose();
    }

    return true;
}

void VulkanIndexBuffer::dispose()
{
    m_buffer.dispose();
}

void VulkanIndexBuffer::setSubData(size_t offset, const void* data, size_t length)
{
    // TODO: これは廃止して、CommandList 側に持って行った方がいいと思う。
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// VulkanTexture2D

VulkanTexture2D::VulkanTexture2D()
{
}

Result VulkanTexture2D::init(VulkanDeviceContext* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;
    m_size.width = width;
    m_size.height = height;
    m_format = requestFormat;

    VkFormat nativeFormat = VulkanHelper::LNFormatToVkFormat(requestFormat);
    VkDeviceSize imageSize = width * height * GraphicsHelper::getPixelSize(requestFormat);

    if (!initialData) {
        LN_NOTIMPLEMENTED();
    }
    else {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_deviceContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
            return false;
        }
        stagingBuffer.setData(0, initialData, imageSize);

        // VK_FORMAT_R8G8B8A8_UNORM
        m_image.init(m_deviceContext, width, height, nativeFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

        if (!m_deviceContext->transitionImageLayout(m_image.vulkanImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
            return false;
        }

        m_deviceContext->copyBufferToImageImmediately(stagingBuffer.vulkanBuffer(), m_image.vulkanImage(), width, height);

        if (!m_deviceContext->transitionImageLayout(m_image.vulkanImage(), VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
            return false;
        }

        stagingBuffer.dispose();
    }

	return true;
}

void VulkanTexture2D::dispose()
{
    m_image.dispose();
}


//==============================================================================
// VulkanSwapchainRenderTargetTexture

VulkanRenderTarget::VulkanRenderTarget()
    : m_deviceContext(nullptr)
{
}

Result VulkanRenderTarget::init(VulkanDeviceContext* deviceContext, uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;
	return reset(width, height, format, image, imageView);
}

void VulkanRenderTarget::dispose()
{
    if (m_deviceContext) {
        m_deviceContext->framebufferCache()->invalidateRenderTarget(this);
        m_deviceContext = nullptr;
    }
    VulkanTexture::dispose();
}

Result VulkanRenderTarget::reset(uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView)
{
	m_size.width = width;
	m_size.height = height;
	m_format = VulkanHelper::VkFormatToLNFormat(format);

	m_image = std::make_unique<VulkanImage>();
	if (!m_image->init(m_deviceContext, format, image, imageView)) {
		return false;
	}
	return true;
}

//==============================================================================
// VulkanSwapchainRenderTargetTexture

//VulkanSwapchainRenderTargetTexture::VulkanSwapchainRenderTargetTexture()
//{
//}
//
//Result VulkanSwapchainRenderTargetTexture::init(VulkanDeviceContext* deviceContext)
//{
//    LN_DCHECK(deviceContext);
//    m_deviceContext = deviceContext;
//    m_currentBufferIndex = 0;
//    return true;
//}
//
//void VulkanSwapchainRenderTargetTexture::dispose()
//{
//    if (m_deviceContext) {
//        m_deviceContext->framebufferCache()->invalidateRenderTarget(this);
//        m_deviceContext = nullptr;
//    }
//
//    clear();
//    VulkanTexture::dispose();
//}
//
//Result VulkanSwapchainRenderTargetTexture::reset(uint32_t width, uint32_t height, VkFormat format, const std::vector<VkImage>& images, const std::vector<VkImageView>& imageViews)
//{
//    LN_DCHECK(images.size() == imageViews.size());
//    clear();
//
//    m_size.width = width;
//    m_size.height = height;
//    m_format = VulkanHelper::VkFormatToLNFormat(format);
//
//    m_images.resize(images.size());
//    for (int i = 0; i < images.size(); i++) {
//        m_images[i] = std::make_shared<VulkanImage>();
//        if (!m_images[i]->init(m_deviceContext, format, images[i], imageViews[i])) {
//            return false;
//        }
//    }
//
//    return true;
//}
//
//void VulkanSwapchainRenderTargetTexture::clear()
//{
//    for (auto& image : m_images) {
//        image->dispose();
//    }
//    m_images.clear();
//}

//==============================================================================
// VulkanDepthBuffer

VulkanDepthBuffer::VulkanDepthBuffer()
{
}

Result VulkanDepthBuffer::init(VulkanDeviceContext* deviceContext, uint32_t width, uint32_t height)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    VkFormat depthFormat = m_deviceContext->findDepthFormat();

    if (!m_image.init(m_deviceContext, width, height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT)) {
        return false;
    }

    if (!m_deviceContext->transitionImageLayout(m_image.vulkanImage(), depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)) {
        return false;
    }

    return true;
}

void VulkanDepthBuffer::dispose()
{
    if (m_deviceContext) {
        m_deviceContext->framebufferCache()->invalidateDepthBuffer(this);
        m_deviceContext = nullptr;
    }

    m_image.dispose();
}

//==============================================================================
// VulkanSamplerState

VulkanSamplerState::VulkanSamplerState()
	: m_deviceContext(nullptr)
	, m_sampler(VK_NULL_HANDLE)
{
}

Result VulkanSamplerState::init(VulkanDeviceContext* deviceContext, const SamplerStateData& desc)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;

	VkFilter filter = VulkanHelper::LNTextureFilterModeToVkFilter(desc.filter);
	VkSamplerAddressMode address = VulkanHelper::LNTextureAddressModeModeToVkSamplerAddressMode(desc.address);

	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = filter;
	samplerInfo.minFilter = filter;
	samplerInfo.addressModeU = address;
	samplerInfo.addressModeV = address;
	samplerInfo.addressModeW = address;
	samplerInfo.anisotropyEnable = VK_TRUE;		// TODO:
	samplerInfo.maxAnisotropy = 16;
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	LN_VK_CHECK(vkCreateSampler(m_deviceContext->vulkanDevice(), &samplerInfo, m_deviceContext->vulkanAllocator(), &m_sampler));

	return true;
}

void VulkanSamplerState::dispose()
{
	if (m_sampler) {
		vkDestroySampler(m_deviceContext->vulkanDevice(), m_sampler, m_deviceContext->vulkanAllocator());
		m_sampler = VK_NULL_HANDLE;
	}

	m_deviceContext = nullptr;
	ISamplerState::dispose();
}

//==============================================================================
// VulkanShaderPass

VulkanShaderPass::VulkanShaderPass()
{
}

Result VulkanShaderPass::init(VulkanDeviceContext* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    VkDevice device = m_deviceContext->vulkanDevice();

    // vert
    {
        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = createInfo.vsCodeLen;
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.vsCode);

        LN_VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, m_deviceContext->vulkanAllocator(), &m_vertShaderModule));
    }

    // frag
    {
        VkShaderModuleCreateInfo shaderCreateInfo = {};
        shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize = createInfo.psCodeLen;
        shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.psCode);

        LN_VK_CHECK(vkCreateShaderModule(device, &shaderCreateInfo, m_deviceContext->vulkanAllocator(), &m_fragShaderModule));
    }

	// SamplerBuffer
	m_localShaderSamplerBuffer = makeRef<VulkanLocalShaderSamplerBuffer>();
	if (!m_localShaderSamplerBuffer->init()) {
		return false;
	}

    // DescriptorSetLayout
    {
        // https://docs.microsoft.com/ja-jp/windows/desktop/direct3dhlsl/dx-graphics-hlsl-variable-register

        // 'b' register in HLSL
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->uniformBufferRegister.size());
            m_bufferDescriptorBufferInfo.reserve(createInfo.descriptorLayout->uniformBufferRegister.size());
            for (auto& item : createInfo.descriptorLayout->uniformBufferRegister) {
                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromVertexStage(DescriptorType_UniformBuffer)) ? VK_SHADER_STAGE_VERTEX_BIT : 0;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromPixelStage(DescriptorType_UniformBuffer)) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                VkDescriptorBufferInfo info;
                info.buffer = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                info.offset = 0;
                info.range = item.size;
                m_bufferDescriptorBufferInfo.push_back(info);
                
                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                set.pImageInfo = nullptr;
                set.pBufferInfo = &m_bufferDescriptorBufferInfo.back();
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[0]));
        }

        // 't' register in HLSL
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->textureRegister.size());
            m_textureDescripterImageInfo.reserve(createInfo.descriptorLayout->textureRegister.size());
            for (auto& item : createInfo.descriptorLayout->textureRegister) {
                m_localShaderSamplerBuffer->addDescriptor(DescriptorType_Texture, item.binding, item.name, layoutBindings.size(), m_descriptorWriteInfo.size());

                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromVertexStage(DescriptorType_Texture)) ? VK_SHADER_STAGE_VERTEX_BIT : 0;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromPixelStage(DescriptorType_Texture)) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                VkDescriptorImageInfo info;
                info.sampler = VK_NULL_HANDLE;
                info.imageView = VK_NULL_HANDLE;    // set from submitDescriptorWriteInfo
                info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                m_textureDescripterImageInfo.push_back(info);

                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                set.pImageInfo = &m_textureDescripterImageInfo.back();
                set.pBufferInfo = nullptr;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[1]));
        }

        // 's' register in HLSL (SamplerState and CombinedSampler)
        {
            std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
            layoutBindings.reserve(createInfo.descriptorLayout->samplerRegister.size());
            m_samplerDescripterImageInfo.reserve(createInfo.descriptorLayout->samplerRegister.size());
            for (auto& item : createInfo.descriptorLayout->samplerRegister) {
                m_localShaderSamplerBuffer->addDescriptor(DescriptorType_SamplerState, item.binding, item.name, layoutBindings.size(), m_descriptorWriteInfo.size());

                VkDescriptorSetLayoutBinding layoutBinding = {};
                layoutBinding.binding = item.binding;
                layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;   // VK_DESCRIPTOR_TYPE_SAMPLER としても使える。ただし、ImageView をセットしておく必要がある。
                layoutBinding.descriptorCount = 1;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromVertexStage(DescriptorType_SamplerState)) ? VK_SHADER_STAGE_VERTEX_BIT : 0;
                layoutBinding.stageFlags |= (createInfo.descriptorLayout->isReferenceFromPixelStage(DescriptorType_SamplerState)) ? VK_SHADER_STAGE_FRAGMENT_BIT : 0;
                layoutBinding.pImmutableSamplers = nullptr;
                layoutBindings.push_back(layoutBinding);

                VkDescriptorImageInfo info;
                info.sampler = VK_NULL_HANDLE;      // set from submitDescriptorWriteInfo
                info.imageView = VK_NULL_HANDLE;    // set from submitDescriptorWriteInfo
                info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                m_samplerDescripterImageInfo.push_back(info);

                VkWriteDescriptorSet set;
                set.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                set.pNext = nullptr;
                set.dstSet = VK_NULL_HANDLE;   // set from submitDescriptorWriteInfo
                set.dstBinding = item.binding;
                set.dstArrayElement = 0;
                set.descriptorCount = 1;
                set.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;//VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                set.pImageInfo = &m_samplerDescripterImageInfo.back();
                set.pBufferInfo = nullptr;
                set.pTexelBufferView = nullptr;
                m_descriptorWriteInfo.push_back(set);
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = layoutBindings.size();    // 0 で空のインスタンスだけ作ることは可能
            layoutInfo.pBindings = layoutBindings.data();
            LN_VK_CHECK(vkCreateDescriptorSetLayout(device, &layoutInfo, m_deviceContext->vulkanAllocator(), &m_descriptorSetLayouts[2]));
        }
    }

    // PipelineLayout
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = m_descriptorSetLayouts.size();
        pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();
        LN_VK_CHECK(vkCreatePipelineLayout(device, &pipelineLayoutInfo, m_deviceContext->vulkanAllocator(), &m_pipelineLayout));
    }

	// UniformBuffers
	{
        uint32_t writeIndex = 0;
		for (auto& item : createInfo.descriptorLayout->uniformBufferRegister)
		{
			auto buf = makeRef<VulkanShaderUniformBuffer>();
			if (!buf->init(m_deviceContext, item.name, item.size, item.members)) {
				return false;
			}
            buf->descriptorWriteInfoIndex = writeIndex;
            buf->bindingIndex = item.binding;
            writeIndex++;

			m_uniformBuffers.push_back(buf);
		}
	}

    return true;
}

void VulkanShaderPass::dispose()
{
    if (m_deviceContext) {
        VkDevice device = m_deviceContext->vulkanDevice();

        for (auto& buf : m_uniformBuffers) {
            buf->dispose();
        }
        m_uniformBuffers.clear();
    
        if (m_localShaderSamplerBuffer) {
            m_localShaderSamplerBuffer->dispose();
            m_localShaderSamplerBuffer = nullptr;
        }

        for (auto& pool : m_descriptorSetsPools) {
            pool->dispose();
        }
        m_descriptorSetsPools.clear();

        if (m_localShaderSamplerBuffer) {
            m_localShaderSamplerBuffer->dispose();
            m_localShaderSamplerBuffer = nullptr;
        }

        if (m_pipelineLayout) {
            vkDestroyPipelineLayout(device, m_pipelineLayout, m_deviceContext->vulkanAllocator());
            m_pipelineLayout = VK_NULL_HANDLE;
        }

        for (auto& layout : m_descriptorSetLayouts) {
            if (layout) {
                vkDestroyDescriptorSetLayout(device, layout, m_deviceContext->vulkanAllocator());
                layout = VK_NULL_HANDLE;
            }
        }

        if (m_vertShaderModule) {
            vkDestroyShaderModule(device, m_vertShaderModule, m_deviceContext->vulkanAllocator());
            m_vertShaderModule = VK_NULL_HANDLE;
        }

        if (m_fragShaderModule) {
            vkDestroyShaderModule(device, m_fragShaderModule, m_deviceContext->vulkanAllocator());
            m_fragShaderModule = VK_NULL_HANDLE;
        }

        m_deviceContext->pipelineCache()->invalidateFromShaderPass(this);
        m_deviceContext = nullptr;
    }
}

IShaderUniformBuffer* VulkanShaderPass::getUniformBuffer(int index) const
{
    return m_uniformBuffers[index];
}

IShaderSamplerBuffer* VulkanShaderPass::samplerBuffer() const
{
    return m_localShaderSamplerBuffer;
}

const std::vector<VkWriteDescriptorSet>& VulkanShaderPass::submitDescriptorWriteInfo(VulkanCommandBuffer* commandBuffer, const std::array<VkDescriptorSet, DescriptorType_Count>& descriptorSets)
{
    for (size_t i = 0; i < m_uniformBuffers.size(); i++) {
        auto& uniformBuffer = m_uniformBuffers[i];

        // UniformBuffer の内容を CopyCommand に乗せる
        VulkanBuffer* buffer = commandBuffer->cmdCopyBuffer(uniformBuffer->buffer()->size(), uniformBuffer->buffer());
        buffer->setData(0, uniformBuffer->data().data(), uniformBuffer->data().size());

        VkDescriptorBufferInfo& info = m_bufferDescriptorBufferInfo[i];
        info.buffer = uniformBuffer->vulkanBuffer();

        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[i];
        writeInfo.dstSet = descriptorSets[DescriptorType_UniformBuffer];
		i++;
    }

    int count = m_localShaderSamplerBuffer->registerCount();
    for (int i = 0; i < count; i++) {
        DescriptorType type = m_localShaderSamplerBuffer->descriptorType(i);
        uint32_t imageIndex = m_localShaderSamplerBuffer->descriptorImageInfoIndex(i);
        uint32_t writeIndex = m_localShaderSamplerBuffer->descriptorWriteInfoIndex(i);
        VkWriteDescriptorSet& writeInfo = m_descriptorWriteInfo[writeIndex];

        auto& texture = m_localShaderSamplerBuffer->texture(i);
        auto& samplerState = m_localShaderSamplerBuffer->samplerState(i);

        if (type == DescriptorType_Texture) {
            VkDescriptorImageInfo& imageInfo = m_textureDescripterImageInfo[imageIndex];
            imageInfo.imageView = (texture) ? texture->image()->vulkanImageView() : 0;
            writeInfo.dstSet = descriptorSets[DescriptorType_Texture];
        }
        else if (type == DescriptorType_SamplerState) {
            VkDescriptorImageInfo& imageInfo = m_samplerDescripterImageInfo[imageIndex];
            imageInfo.imageView = (texture) ? texture->image()->vulkanImageView() : 0;
            imageInfo.sampler = (samplerState) ? samplerState->vulkanSampler() : 0;
            writeInfo.dstSet = descriptorSets[DescriptorType_SamplerState];
        }
        else {
            LN_UNREACHABLE();
        }
    }

    return m_descriptorWriteInfo;
}

Ref<VulkanDescriptorSetsPool> VulkanShaderPass::getDescriptorSetsPool()
{
    if (m_descriptorSetsPools.empty()) {
        auto ptr = makeRef<VulkanDescriptorSetsPool>();
        if (!ptr->init(m_deviceContext, this)) {
            return nullptr;
        }
        return ptr;
    }
    else {
        auto ptr = m_descriptorSetsPools.back();
        m_descriptorSetsPools.pop_back();
        return ptr;
    }
}

void VulkanShaderPass::releaseDescriptorSetsPool(VulkanDescriptorSetsPool* pool)
{
    LN_DCHECK(pool);
    pool->reset();
    m_descriptorSetsPools.push_back(pool);
}

//==============================================================================
// VulkanShaderUniformBuffer

VulkanShaderUniformBuffer::VulkanShaderUniformBuffer()
{
}

Result VulkanShaderUniformBuffer::init(VulkanDeviceContext* deviceContext, const std::string& name, size_t size, const std::vector<ShaderUniformInfo>& members)
{
	m_name = name;
	if (m_name == "$Global") {
		m_name = "_Global";
	}

	m_data.resize(size);

    // TRANSFER_DST に最適化
    if (!m_uniformBuffer.init(deviceContext, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
        return false;
    }

	return true;
}

void VulkanShaderUniformBuffer::dispose()
{
    m_uniformBuffer.dispose();
	IShaderUniformBuffer::dispose();
}

void VulkanShaderUniformBuffer::setData(const void* data, size_t size)
{
	LN_DCHECK(size <= m_data.size());
	memcpy(m_data.data(), data, size);
}

//=============================================================================
// VulkanLocalShaderSamplerBuffer

VulkanLocalShaderSamplerBuffer::VulkanLocalShaderSamplerBuffer()
{
}

Result VulkanLocalShaderSamplerBuffer::init(/*const DescriptorLayout* descriptorLayout*/)
{
    //uint32_t writeIndex = descriptorLayout->uniformBufferRegister.size();

    //// 't' register in HLSL
    //for (auto& item : descriptorLayout->textureRegister) {
    //    Entry e;
    //    e.textureRegisterName = item.name;
    //    e.descriptorWriteInfoIndex = writeIndex;
    //    e.bindingIndex = item.binding;
    //    m_table.push_back(e);
    //    writeIndex++;
    //}

    //// 's' register in HLSL (SamplerState and CombinedSampler)
    //for (auto& item : descriptorLayout->samplerRegister) {
    //    Entry e;
    //    e.samplerRegisterName = item.name;
    //    e.descriptorWriteInfoIndex = writeIndex;
    //    e.bindingIndex = item.binding;
    //    m_table.push_back(e);
    //    writeIndex++;
    //}

    return true;
}

void VulkanLocalShaderSamplerBuffer::dispose()
{
}

void VulkanLocalShaderSamplerBuffer::setTexture(int registerIndex, ITexture* texture)
{
    m_table[registerIndex].texture = static_cast<VulkanTexture*>(texture);
}

void VulkanLocalShaderSamplerBuffer::setSamplerState(int registerIndex, ISamplerState* state)
{
    m_table[registerIndex].samplerState = static_cast<VulkanSamplerState*>(state);
}

void VulkanLocalShaderSamplerBuffer::addDescriptor(DescriptorType type, uint32_t bindingIndex, const std::string& name, uint32_t imageInfoIndex, uint32_t writeInfoIndex)
{
	Entry e;
    e.type = type;
	if (type == DescriptorType_Texture) {
		e.textureRegisterName = name;
	}
	else {
		e.samplerRegisterName = name;
	}
    e.descriptorImageInfoIndex = imageInfoIndex;
	e.descriptorWriteInfoIndex = writeInfoIndex;
	e.bindingIndex = bindingIndex;
	m_table.push_back(std::move(e));
}

} // namespace detail
} // namespace ln

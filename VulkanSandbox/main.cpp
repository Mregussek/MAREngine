
#include "VulkanInclude.h"
#include "VulkanLogging.h"
#include "src/Vulkan/ContextVulkan.h"
#include "src/Vulkan/SwapchainVulkan.h"
#include "src/Vulkan/BufferVulkan.h"
#include "src/Vulkan/WindowSurfaceVulkan.h"
#include "src/Vulkan/DeviceQueueVulkan.h"
#include "src/Vulkan/ShaderVulkan.h"
#include "src/Vulkan/GraphicsPipelineVulkan.h"
#include "src/Vulkan/CommandManagerVulkan.h"
#include "src/Window/Window.h"
#include "src/Mesh/Mesh.h"



int main(void) {
    mar::Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    mar::ContextVulkan contextVk{};
    contextVk.initialize();

    mar::WindowSurfaceVulkan windowSurfaceVk{};
    windowSurfaceVk.create();

    VkExtent2D windowSize{ (uint32_t)window.getWidth() , (uint32_t)window.getHeight() };

    const VkViewport viewport{ 0.f , (float)window.getHeight(), (float)window.getWidth(), -(float)window.getHeight(), 0.f, 1.f };
    const VkRect2D scissor{ {0, 0 }, windowSize };

    mar::CommandManagerVulkan commandManagerVk{};
    commandManagerVk.create(windowSurfaceVk.getSwapchainFormat());
    const auto renderPass{ commandManagerVk.getRenderPass() };

    mar::DeviceQueueVulkan deviceQueueVk{};
    deviceQueueVk.create();

    mar::ShaderCollectionVulkan shaderCollectionVk{};
    auto& vertexShader = shaderCollectionVk.getVertex();
    vertexShader.load("resources/triangle.vert.spv");
    auto& fragmentShader = shaderCollectionVk.getFragment();
    fragmentShader.load("resources/triangle.frag.spv");

    mar::GraphicsPipelineVulkan graphicsPipelineVk{};
    graphicsPipelineVk.create(shaderCollectionVk, renderPass, viewport, scissor);

    mar::SwapchainVulkan swapchainVk{};
    swapchainVk.create(windowSurfaceVk, renderPass);

    mar::Mesh mesh{};
    mesh.loadFromFile("resources/monkey.obj");

    mar::BufferVulkan vertexBuffer{};
    vertexBuffer.create(128 * 1024 * 1024, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    vertexBuffer.update(mesh.m_vertices);

    mar::BufferVulkan indexBuffer{};
    indexBuffer.create(128 * 1024 * 1024, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    indexBuffer.update(mesh.m_indices);

    constexpr VkClearColorValue clearColor{ 48.f / 255.f, 10.f / 255.f, 36.f / 255.f , 1.f };
    constexpr VkClearValue clearValue{ clearColor };

    while (window.shouldClose()) {
        windowSurfaceVk.update();

        const bool windowSizeHasChanged = !(swapchainVk.m_extent.width == windowSurfaceVk.getWidth() && swapchainVk.m_extent.height == windowSurfaceVk.getHeight());
        if (windowSizeHasChanged) {
            swapchainVk.resizeIfNecessary(windowSurfaceVk, renderPass);
            windowSize.height = window.getHeight();
            windowSize.width = window.getWidth();
        }

        uint32_t imageIndex{ 0 };
        deviceQueueVk.prepare(swapchainVk.swapchain, imageIndex);

        commandManagerVk.prepareFrame(swapchainVk, clearValue, imageIndex);

        vkCmdSetViewport(commandManagerVk.m_commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandManagerVk.m_commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandManagerVk.m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineVk.m_pipeline);

        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = vertexBuffer.m_buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = vertexBuffer.m_size;

        std::array<VkWriteDescriptorSet, 1> descriptors{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        descriptors[0].dstBinding = 0;
        descriptors[0].descriptorCount = 1;
        descriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptors[0].pBufferInfo = &bufferInfo;
        descriptors[0].pNext = nullptr;

        vkCmdPushDescriptorSetKHR(commandManagerVk.m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineVk.m_pipelineLayout, 0, descriptors.size(), descriptors.data());

        vkCmdBindIndexBuffer(commandManagerVk.m_commandBuffer, indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandManagerVk.m_commandBuffer, (uint32_t)mesh.m_indices.size(), 1, 0, 0, 0);

        commandManagerVk.endFrame(swapchainVk, imageIndex);

        deviceQueueVk.draw(commandManagerVk.m_commandBuffer, swapchainVk.swapchain, imageIndex);
        
        contextVk.endPendingJobs();

        window.pollEvents();
    }

    contextVk.endPendingJobs();

    vertexBuffer.close();
    indexBuffer.close();

    swapchainVk.close();

    deviceQueueVk.close();

    graphicsPipelineVk.close();

    shaderCollectionVk.close();

    commandManagerVk.close();

    windowSurfaceVk.close();

    contextVk.terminate();

    window.terminate();

    return 0;
}
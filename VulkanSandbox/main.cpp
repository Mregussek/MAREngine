
#include "VulkanInclude.h"
#include "VulkanLogging.h"
#include "src/Vulkan/ContextVulkan.h"
#include "src/Vulkan/PhysicalDevVulkan.h"
#include "src/Vulkan/LogicalDevVulkan.h"
#include "src/Vulkan/SwapchainVulkan.h"
#include "src/Vulkan/BufferVulkan.h"
#include "src/Vulkan/WindowSurfaceVulkan.h"
#include "src/Vulkan/DeviceQueueVulkan.h"
#include "src/Vulkan/ShaderVulkan.h"
#include "src/Vulkan/GraphicsPipelineVulkan.h"
#include "src/Window/Window.h"
#include "src/Mesh/Mesh.h"


VkCommandPool createCommandPool(VkDevice device, uint32_t familyIndex) {
    VkCommandPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    createInfo.queueFamilyIndex = familyIndex;

    VkCommandPool commandPool{ 0 };
    VK_CHECK( vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) );

    return commandPool;
}

VkRenderPass createRenderPass(VkDevice device, VkFormat format) {
    std::array<VkAttachmentDescription, 1> attachments;
    attachments[0].format = format;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[0].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

    VkAttachmentReference colorAttachment{ 0 , VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachment;

    VkRenderPassCreateInfo createInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
    createInfo.attachmentCount = attachments.size();
    createInfo.pAttachments = attachments.data();
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;

    VkRenderPass renderPass{ 0 };
    VK_CHECK( vkCreateRenderPass(device, &createInfo, nullptr, &renderPass) );
    
    return renderPass;
}

VkCommandBuffer createCommandBuffer(VkDevice device, VkCommandPool commandPool) {
    VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer{ 0 };

    VK_CHECK(vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer));

    return commandBuffer;
}

VkImageMemoryBarrier pipelineBarrierImage(VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldLayout, VkImageLayout newLayout)  {
    VkImageMemoryBarrier result{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    result.srcAccessMask = srcAccessMask;
    result.dstAccessMask = dstAccessMask;
    result.oldLayout = oldLayout;
    result.newLayout = newLayout;
    result.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    result.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    result.image = image;
    result.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    result.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;       // those flags may not be supported on android!
    result.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

    return result;
}

int main(void) {
    mar::Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    mar::ContextVulkan contextVk{};
    contextVk.create();
    
    mar::PhysicalDevVulkan physicalDevVk{};
    physicalDevVk.create();

    mar::LogicalDevVulkan deviceVk{};
    deviceVk.create();

    mar::WindowSurfaceVulkan windowSurfaceVk{};
    windowSurfaceVk.create();

    VkExtent2D windowSize{ (uint32_t)window.getWidth() , (uint32_t)window.getHeight() };

    const VkViewport viewport{ 0.f , (float)window.getHeight(), (float)window.getWidth(), -(float)window.getHeight(), 0.f, 1.f };
    const VkRect2D scissor{ {0, 0 }, windowSize };

    const auto renderPass{ createRenderPass(deviceVk.getDev(), windowSurfaceVk.getSwapchainFormat()) };
    const auto commandPool{ createCommandPool(deviceVk.getDev(), physicalDevVk.getFamilyIndex()) };
    const auto commandBuffer{ createCommandBuffer(deviceVk.getDev(), commandPool) };
    
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

        VK_CHECK( vkResetCommandPool(deviceVk.getDev(), commandPool, 0) );

        VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK( vkBeginCommandBuffer(commandBuffer, &beginInfo) );
        
        const auto renderBeginBarrier{ pipelineBarrierImage(swapchainVk.images[imageIndex], 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &renderBeginBarrier);

        constexpr VkClearColorValue clearColor{ 48.f / 255.f, 10.f / 255.f, 36.f / 255.f ,1 };
        constexpr std::array<VkClearValue, 1> clearValue{ clearColor };

        VkRenderPassBeginInfo passBeginInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        passBeginInfo.renderPass = renderPass;
        passBeginInfo.framebuffer = swapchainVk.framebuffers[imageIndex];
        passBeginInfo.renderArea.extent = swapchainVk.m_extent;
        passBeginInfo.clearValueCount = clearValue.size();
        passBeginInfo.pClearValues = clearValue.data();

        vkCmdBeginRenderPass(commandBuffer, &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE); // draw calls are below

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineVk.m_pipeline);

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

        vkCmdPushDescriptorSetKHR(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineVk.m_pipelineLayout, 0, descriptors.size(), descriptors.data());

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, (uint32_t)mesh.m_indices.size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        const auto renderEndBarrier{ pipelineBarrierImage(swapchainVk.images[imageIndex], VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &renderEndBarrier);

        VK_CHECK( vkEndCommandBuffer(commandBuffer) );

        deviceQueueVk.draw(commandBuffer, swapchainVk.swapchain, imageIndex);
        
        deviceVk.endPendingJobs();

        window.pollEvents();
    }

    deviceVk.endPendingJobs();

    vertexBuffer.close();
    indexBuffer.close();

    swapchainVk.close();

    deviceQueueVk.close();

    graphicsPipelineVk.close();

    shaderCollectionVk.close();

    vkDestroyCommandPool(deviceVk.getDev(), commandPool, nullptr);

    vkDestroyRenderPass(deviceVk.getDev(), renderPass, nullptr);

    windowSurfaceVk.close();

    deviceVk.close();

    contextVk.close();

    window.terminate();

    exit(EXIT_SUCCESS);
}
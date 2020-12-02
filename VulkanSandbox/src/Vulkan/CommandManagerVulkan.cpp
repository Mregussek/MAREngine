

#include "CommandManagerVulkan.h"
#include "PhysicalDevVulkan.h"
#include "LogicalDevVulkan.h"
#include "SwapchainVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


    void CommandManagerVulkan::create(VkFormat format) {
        createRenderPass(format);
        createCommandPool();
        createCommandBuffer();
    }

    void CommandManagerVulkan::close() {
        const auto& device = LogicalDevVulkan::Instance()->getDev();
        vkDestroyCommandPool(device, m_commandPool, nullptr);
        vkDestroyRenderPass(device, m_renderPass, nullptr);
    }

    void CommandManagerVulkan::prepareFrame(const SwapchainVulkan& swapchain, VkClearValue clearValue, uint32_t imageIndex) {
        VK_CHECK(vkResetCommandPool(LogicalDevVulkan::Instance()->getDev(), m_commandPool, 0));

        VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK(vkBeginCommandBuffer(m_commandBuffer, &beginInfo));

        const auto beginBarrier{ beginRenderBarrier(swapchain.images[imageIndex]) };
        vkCmdPipelineBarrier(m_commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &beginBarrier);

        VkRenderPassBeginInfo passBeginInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        passBeginInfo.renderPass = m_renderPass;
        passBeginInfo.framebuffer = swapchain.framebuffers[imageIndex];
        passBeginInfo.renderArea.extent = swapchain.m_extent;
        passBeginInfo.clearValueCount = 1;
        passBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(m_commandBuffer, &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandManagerVulkan::endFrame(const SwapchainVulkan& swapchain, uint32_t imageIndex) {
        vkCmdEndRenderPass(m_commandBuffer);

        const auto endBarrier{ endRenderBarrier(swapchain.images[imageIndex]) };

        vkCmdPipelineBarrier(m_commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &endBarrier);

        VK_CHECK(vkEndCommandBuffer(m_commandBuffer));
    }

    VkRenderPass CommandManagerVulkan::getRenderPass() const {
        return m_renderPass;
    }

    void CommandManagerVulkan::createRenderPass(VkFormat format) {
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

        VK_CHECK(vkCreateRenderPass(LogicalDevVulkan::Instance()->getDev(), &createInfo, nullptr, &m_renderPass));
    }

    void CommandManagerVulkan::createCommandPool() {
        VkCommandPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
        createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        createInfo.queueFamilyIndex = PhysicalDevVulkan::Instance()->getFamilyIndex();

        VK_CHECK(vkCreateCommandPool(LogicalDevVulkan::Instance()->getDev(), &createInfo, nullptr, &m_commandPool));
    }

    void CommandManagerVulkan::createCommandBuffer() {
        VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
        allocateInfo.commandPool = m_commandPool;
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;

        VK_CHECK(vkAllocateCommandBuffers(LogicalDevVulkan::Instance()->getDev(), &allocateInfo, &m_commandBuffer));
    }

    VkImageMemoryBarrier CommandManagerVulkan::beginRenderBarrier(VkImage image) {
        VkImageMemoryBarrier result{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
        result.srcAccessMask = VK_NULL_HANDLE;
        result.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        result.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        result.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        result.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        result.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        result.image = image;
        result.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        result.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;       // those flags may not be supported on android!
        result.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        return result;
    }

    VkImageMemoryBarrier CommandManagerVulkan::endRenderBarrier(VkImage image) {
        VkImageMemoryBarrier result{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
        result.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        result.dstAccessMask = VK_NULL_HANDLE;
        result.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        result.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        result.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        result.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        result.image = image;
        result.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        result.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;       // those flags may not be supported on android!
        result.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

        return result;
    }


}



#include "ContextVulkan.h"
#include "../Window/Window.h"
#include "../../VulkanLogging.h"


namespace mar {
    

    void ContextVulkan::initialize(Window* pWindow) {
        m_pWindow = pWindow;

        createInstance();
        createPhysicalDevice();
        createLogicalDevice();
        createWindowSurface();
        createRenderPass();
        createCommandPool();
        createCommandBuffer();
        createDeviceQueue();

        m_viewport = { 0.f , (float)m_pWindow->getHeight(), (float)m_pWindow->getWidth(), -(float)m_pWindow->getHeight(), 0.f, 1.f };
        m_scissor = { {0, 0 }, { (uint32_t)m_pWindow->getWidth() , (uint32_t)m_pWindow->getHeight() } };

        createSwapchain();
    }

    void ContextVulkan::terminate() {

        closeSwapchain();
        closeDeviceQueue();
        closeCommandPool();
        closeRenderPass();
        closeWindowSurface();
        closeLogicalDevice();
        closeInstance();

        m_pWindow = nullptr;
    }

    void ContextVulkan::endPendingJobs() const {
        VK_CHECK( vkDeviceWaitIdle(m_device) );
    }

    void ContextVulkan::beginFrame() {
        updateWindowSurface();

        const bool windowSizeHasChanged = !(
            m_extent.width == m_surfaceCaps.currentExtent.width &&
            m_extent.height == m_surfaceCaps.currentExtent.height
        );
        if (windowSizeHasChanged) {
            resizeSwapchain();
        }

        VK_CHECK( vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_acquireSemaphore, VK_NULL_HANDLE, &m_imageIndex) );

        VK_CHECK( vkResetCommandPool(m_device, m_commandPool, 0) );

        beginCommandBuffer();
        beginRenderPass();
    
        vkCmdSetViewport(m_commandBuffer, 0, 1, &m_viewport);
        vkCmdSetScissor(m_commandBuffer, 0, 1, &m_scissor);
    }

    void ContextVulkan::updateFrame() {
        
    }

    void ContextVulkan::endFrame() {
        endRenderPass();
        endCommandBuffer();

        constexpr VkPipelineStageFlags submitStageMask{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_acquireSemaphore;
        submitInfo.pWaitDstStageMask = &submitStageMask;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_releaseSemaphore;

        VK_CHECK(vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE));

        VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_swapchain;
        presentInfo.pImageIndices = &m_imageIndex;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_releaseSemaphore;

        VK_CHECK(vkQueuePresentKHR(m_queue, &presentInfo));
    }

    uint32_t ContextVulkan::getApiVersion() {
        uint32_t apiVersion{ 0 };
        VK_CHECK( vkEnumerateInstanceVersion(&apiVersion) );

        //std::cout << "Supported version: " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) << "." << VK_VERSION_PATCH(apiVersion) << "\n";

        return apiVersion;
    }

    const VkCommandBuffer& ContextVulkan::getCommandBuffer() const {
        return m_commandBuffer;
    }

    const VkRenderPass& ContextVulkan::getRenderPass() const {
        return m_renderPass;
    }

    const VkViewport& ContextVulkan::getViewport() const {
        return m_viewport;
    }

    const VkRect2D& ContextVulkan::getScissor() const {
        return m_scissor;
    }

    const VkDevice& ContextVulkan::getLogicalDevice() const {
        return m_device;
    }

    const VkPhysicalDeviceMemoryProperties& ContextVulkan::getMemoryProperties() const {
        return m_memoryProperties;
    }

}

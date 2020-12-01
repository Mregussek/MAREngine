

#include "DeviceQueueVulkan.h"
#include "LogicalDevVulkan.h"
#include "PhysicalDevVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


	void DeviceQueueVulkan::create() {

		vkGetDeviceQueue(LogicalDevVulkan::Instance()->getDev(), PhysicalDevVulkan::Instance()->getFamilyIndex(), 0, &m_queue);
			
        createSemaphore(m_acquireSemaphore);
        createSemaphore(m_releaseSemaphore);
	}

	void DeviceQueueVulkan::close() {
        const auto& device = LogicalDevVulkan::Instance()->getDev();

        vkDestroySemaphore(device, m_releaseSemaphore, nullptr);
        vkDestroySemaphore(device, m_acquireSemaphore, nullptr);
	}

    void DeviceQueueVulkan::prepare(const VkSwapchainKHR& swapchain, uint32_t& imageIndex) const {
        VK_CHECK( vkAcquireNextImageKHR(LogicalDevVulkan::Instance()->getDev(), swapchain, UINT64_MAX, m_acquireSemaphore, VK_NULL_HANDLE, &imageIndex) );
    }

	void DeviceQueueVulkan::draw(const VkCommandBuffer& commandBuffer, const VkSwapchainKHR& swapchain, const uint32_t& imageIndex) const {
        constexpr VkPipelineStageFlags submitStageMask{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_acquireSemaphore;
        submitInfo.pWaitDstStageMask = &submitStageMask;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_releaseSemaphore;

        VK_CHECK( vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE) );

        VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_releaseSemaphore;

        VK_CHECK(vkQueuePresentKHR(m_queue, &presentInfo));

	}

    void DeviceQueueVulkan::createSemaphore(VkSemaphore& semaphore) const {
        VkSemaphoreCreateInfo createInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

        VK_CHECK( vkCreateSemaphore(LogicalDevVulkan::Instance()->getDev(), &createInfo, nullptr, &semaphore) );
    }


}
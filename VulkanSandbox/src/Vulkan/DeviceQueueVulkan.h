

#ifndef MAR_ENGINE_PLATFORM_VULKAN_DEVICE_QUEUE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_DEVICE_QUEUE_VULKAN_H


#include "../../VulkanInclude.h"



namespace mar {

	class SwapchainVulkan;



	class DeviceQueueVulkan {
	public:

		void create();
		void close();

		void prepare(const VkSwapchainKHR& swapchain, uint32_t& imageIndex) const;
		void draw(const VkCommandBuffer& commandBuffer, const VkSwapchainKHR& swapchain, const uint32_t& imageIndex) const;


	private:

		void createSemaphore(VkSemaphore& semaphore) const;


		VkQueue m_queue{ VK_NULL_HANDLE };

		VkSemaphore m_acquireSemaphore{ VK_NULL_HANDLE };
		VkSemaphore m_releaseSemaphore{ VK_NULL_HANDLE };

	};



}



#endif // !MAR_ENGINE_PLATFORM_VULKAN_DEVICE_QUEUE_VULKAN_H

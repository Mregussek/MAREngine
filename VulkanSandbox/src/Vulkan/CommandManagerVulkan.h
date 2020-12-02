

#ifndef MAR_ENGINE_PLATFORM_VULKAN_COMMAND_MANAGER_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_COMMAND_MANAGER_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

	class SwapchainVulkan;


	class CommandManagerVulkan {
	public:

		void create(VkFormat format);
		void close();

		void prepareFrame(const SwapchainVulkan& swapchain, VkClearValue clearValue, uint32_t imageIndex);
		void endFrame(const SwapchainVulkan& swapchain, uint32_t imageIndex);

		VkRenderPass getRenderPass() const;

	private:

		void createRenderPass(VkFormat format);
		void createCommandPool();
		void createCommandBuffer();

		VkImageMemoryBarrier beginRenderBarrier(VkImage image);
		VkImageMemoryBarrier endRenderBarrier(VkImage image);


		VkRenderPass m_renderPass{ VK_NULL_HANDLE };
		VkCommandPool m_commandPool{ VK_NULL_HANDLE };

	public:

		VkCommandBuffer m_commandBuffer{ VK_NULL_HANDLE };

	};


}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_COMMAND_MANAGER_VULKAN_H

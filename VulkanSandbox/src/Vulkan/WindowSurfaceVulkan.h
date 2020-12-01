

#ifndef MAR_ENGINE_PLATFORM_VULKAN_WINDOW_SURFACE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_WINDOW_SURFACE_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class WindowSurfaceVulkan {
	public:

		void create();
		void close() const;

		VkSurfaceKHR getSurface() const;
		VkPresentModeKHR getSwapchainPresentMode() const;
		VkFormat getSwapchainFormat() const;
		
	private:

		VkFormat getSurfaceFormat() const;
		VkPresentModeKHR getPresentMode() const;


		VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
		VkFormat m_format{ VK_FORMAT_UNDEFINED };
		VkPresentModeKHR m_presentMode{ VK_PRESENT_MODE_FIFO_KHR };

	};


}



#endif // !MAR_ENGINE_PLATFORM_VULKAN_WINDOW_SURFACE_VULKAN_H
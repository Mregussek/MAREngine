

#ifndef MAR_ENGINE_PLATFORM_VULKAN_WINDOW_SURFACE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_WINDOW_SURFACE_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class WindowSurfaceVulkan {
	public:

		void create();
		void close() const;

		void update();

		VkSurfaceKHR getSurface() const;
		VkPresentModeKHR getSwapchainPresentMode() const;
		VkFormat getSwapchainFormat() const;
		const VkSurfaceCapabilitiesKHR& getSurfaceCaps() const;
		
		uint32_t getWidth() const;
		uint32_t getHeight() const;

	private:

		VkFormat getSurfaceFormat() const;
		VkPresentModeKHR getPresentMode() const;


		VkSurfaceCapabilitiesKHR m_surfaceCaps{ VK_NULL_HANDLE };
		VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
		VkFormat m_format{ VK_FORMAT_UNDEFINED };
		VkPresentModeKHR m_presentMode{ VK_PRESENT_MODE_FIFO_KHR };

	};


}



#endif // !MAR_ENGINE_PLATFORM_VULKAN_WINDOW_SURFACE_VULKAN_H
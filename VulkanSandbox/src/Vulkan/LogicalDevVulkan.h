

#ifndef MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class LogicalDevVulkan {
	public:

		VkDevice m_device{ VK_NULL_HANDLE };

		void create();
		void close();

	private:

		void fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& extensionsToEnable);

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H

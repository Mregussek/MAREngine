

#ifndef MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class DeviceVulkan {
	public:

		VkDevice m_device{ VK_NULL_HANDLE };

		void create(VkPhysicalDevice physicalDevice, uint32_t familyIndex);
		void close();

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H

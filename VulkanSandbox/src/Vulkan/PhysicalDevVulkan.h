

#ifndef MAR_ENGINE_PLATFORM_VULKAN_PHYSICAL_DEV_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_PHYSICAL_DEV_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

	class DeviceVulkan;


	class PhysicalDevVulkan {

		friend class DeviceVulkan;

	public:

		VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
		uint32_t m_familyIndex{ 0 };

		void create(VkInstance instance);

	private:

		bool supportPresentation(VkPhysicalDevice physicalDevice, uint32_t familyIndex);

        uint32_t getGraphicsQueueFamily(VkPhysicalDevice physicalDevice);

        VkPhysicalDevice selectPhysicalDevice(VkInstance instance);

		static void fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& extensionsToEnable);

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_PHYSICAL_DEV_VULKAN_H

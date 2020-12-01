

#ifndef MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class LogicalDevVulkan {
	public:

		static LogicalDevVulkan* Instance();

		void create();
		void close();

		void endPendingJobs() const;

		const VkDevice& getDev() const;

	private:

		void fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& extensionsToEnable);

		
		static LogicalDevVulkan* s_instance;

		VkDevice m_device{ VK_NULL_HANDLE };

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_DEVICE_VULKAN_H

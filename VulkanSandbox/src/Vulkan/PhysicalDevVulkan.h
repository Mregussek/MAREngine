

#ifndef MAR_ENGINE_PLATFORM_VULKAN_PHYSICAL_DEV_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_PHYSICAL_DEV_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class PhysicalDevVulkan {
	public:

		static PhysicalDevVulkan* Instance();

		
		PhysicalDevVulkan() = default;

		void create();

		const VkPhysicalDevice& getPhyDev() const;
		const uint32_t& getFamilyIndex() const;

	private:

		bool supportPresentation(VkPhysicalDevice physicalDevice, uint32_t familyIndex) const;

        uint32_t getGraphicsQueueFamily(VkPhysicalDevice physicalDevice, bool displayInfo) const;

        VkPhysicalDevice selectPhysicalDevice() const;

		void checkDeviceLayerProperties(std::vector<const char*>& layersToEnable) const;

		void displayInfoAboutPhysicalDevice(VkPhysicalDevice physicalDevice, const char* message) const;
		void displayInfoAboutQueueFamilyIndex(const VkQueueFlags& queueFlags) const;


		static PhysicalDevVulkan* s_instance;

		VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
		uint32_t m_familyIndex{ 0 };

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_PHYSICAL_DEV_VULKAN_H

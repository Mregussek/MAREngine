

#ifndef MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class ContextVulkan {
    public:

        static uint32_t getApiVersion();

        void initialize();
        void terminate() const;

        void endPendingJobs() const;

    private:

        void registerDebugCallback();

        void createInstance();
        void createPhysicalDevice();
        void createLogicalDevice();


        // Instance stuff

        VkInstance m_instance{ VK_NULL_HANDLE };
        VkDebugReportCallbackEXT m_callback{ VK_NULL_HANDLE };

        // Physical Device stuff

        VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
        VkQueueFamilyProperties m_familyQueueProperties{ VK_NULL_HANDLE };
        VkPhysicalDeviceMemoryProperties m_memoryProperties{ VK_NULL_HANDLE };
        uint32_t m_familyIndex{ 0 };

        // Logical Device stuff

        VkDevice m_device{ VK_NULL_HANDLE };

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H
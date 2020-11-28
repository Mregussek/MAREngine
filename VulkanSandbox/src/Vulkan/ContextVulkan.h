

#ifndef MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

    class PhysicalDevVulkan;


	class ContextVulkan {

        friend class PhysicalDevVulkan;

    public:

        VkInstance m_instance{ VK_NULL_HANDLE };
        VkDebugReportCallbackEXT m_callback{ VK_NULL_HANDLE };

        void create();
        void close();

    private:

        uint32_t getVulkanApiVersion();

        static bool pushExtensionIfAvailable(std::vector<const char*>& extensionsToEnable, std::vector<VkExtensionProperties>& allExtensions, const char* extensionProperty);

        void fillNeededInstanceExtensions(std::vector<const char*>& extensionToEnable);

        void fillNeededLayers(std::vector<const char*>& layersToEnable);

        void registerDebugCallback();

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H
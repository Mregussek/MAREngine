

#ifndef MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class ContextVulkan {
    public:

        static ContextVulkan* Instance();

        void create();
        void close() const;

        const VkInstance& get() const;

    private:

        uint32_t getVulkanApiVersion();

        void fillNeededInstanceExtensions(std::vector<const char*>& extensionToEnable) const;
        void fillNeededLayers(std::vector<const char*>& layersToEnable) const;

        void registerDebugCallback();


        static ContextVulkan* s_instance;

        VkInstance m_instance{ VK_NULL_HANDLE };
        VkDebugReportCallbackEXT m_callback{ VK_NULL_HANDLE };

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H


#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


    void ContextVulkan::create() {
        const auto apiVersion = getVulkanApiVersion();

        std::vector<const char*> layersToEnable;
        fillNeededLayers(layersToEnable);

        std::vector<const char*> extensionToEnable;
        fillNeededInstanceExtensions(extensionToEnable);

        VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
        appInfo.apiVersion = apiVersion;
        appInfo.pApplicationName = "Hello, Vulkan!";
        appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
        appInfo.pEngineName = "MAREngine";
        appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);

        VkInstanceCreateInfo createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
        createInfo.pApplicationInfo = &appInfo;
        createInfo.ppEnabledLayerNames = layersToEnable.data();
        createInfo.enabledLayerCount = (uint32_t)layersToEnable.size();
        createInfo.ppEnabledExtensionNames = extensionToEnable.data();
        createInfo.enabledExtensionCount = (uint32_t)extensionToEnable.size();

        VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_instance));

        registerDebugCallback();
    }

    void ContextVulkan::close() {
        auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);

        vkDestroyInstance(m_instance, nullptr);
    }

    uint32_t ContextVulkan::getVulkanApiVersion() {
        uint32_t apiVersion{ 0 };
        VK_CHECK(vkEnumerateInstanceVersion(&apiVersion));

        std::cout << "Supported version: " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) <<
            "." << VK_VERSION_PATCH(apiVersion) << "\n";

        return apiVersion;
    }

    bool ContextVulkan::pushExtensionIfAvailable(std::vector<const char*>& extensionsToEnable, std::vector<VkExtensionProperties>& allExtensions, const char* extensionProperty) {
        const auto availableExtension = [&extensionProperty](const VkExtensionProperties extension) {
            return std::strcmp(extension.extensionName, extensionProperty) == 0;
        };

        if (std::find_if(allExtensions.cbegin(), allExtensions.cend(), availableExtension) != allExtensions.cend()) {
            extensionsToEnable.emplace_back(extensionProperty);
            return true;
        }

        return false;
    }

    void ContextVulkan::fillNeededInstanceExtensions(std::vector<const char*>& extensionToEnable) {
        uint32_t extensionCount{ 0 };
        VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));

        std::vector<VkExtensionProperties> extensionProperties(extensionCount);
        VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data()));

        const bool isAvailableKHRWin32Surface = pushExtensionIfAvailable(extensionToEnable, extensionProperties, VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        const bool isAvailableDebugReport = pushExtensionIfAvailable(extensionToEnable, extensionProperties, VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
        const bool isAvailableKHRSurface = pushExtensionIfAvailable(extensionToEnable, extensionProperties, VK_KHR_SURFACE_EXTENSION_NAME);
    }

    void ContextVulkan::fillNeededLayers(std::vector<const char*>& layersToEnable) {
        uint32_t layerCount{ 0 };
        VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, nullptr));

        std::vector<VkLayerProperties> availableLayers(layerCount);
        VK_CHECK(vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()));

        const auto pushLayerIfAvailable = [&layersToEnable, &availableLayers](const char* layerProperty)->bool {
            const auto availableProperty = [&layerProperty](const VkLayerProperties property) {
                return std::strcmp(property.layerName, layerProperty) == 0;
            };

            if (std::find_if(availableLayers.begin(), availableLayers.end(), availableProperty) != availableLayers.end()) {
                layersToEnable.emplace_back(layerProperty);
                return true;
            }

            return false;
        };

        const bool isAvailableKHRONOSValidation = pushLayerIfAvailable("VK_LAYER_KHRONOS_validation");
    }

    void ContextVulkan::registerDebugCallback() {
        auto callbackFunction = [](VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location,
            int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData) ->VkBool32
        {
            const auto error = flags & VK_DEBUG_REPORT_ERROR_BIT_EXT;
            const auto warning = flags & VK_DEBUG_REPORT_WARNING_BIT_EXT;
            const auto perfomanceWarning = flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;

            if (error) {
                printf("%s : %s\n", "ERROR", pMessage);
                //__debugbreak();
            }
            else if (warning) {
                printf("%s : %s\n", "WARNING", pMessage);
                //__debugbreak();
            }
            else if (perfomanceWarning) {
                printf("%s : %s\n", "PERFOMANCE_WARNING", pMessage);
                //__debugbreak();
            }

            return VK_FALSE;
        };

        VkDebugReportCallbackCreateInfoEXT createInfo{ VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT };
        createInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
        createInfo.pfnCallback = callbackFunction;

        auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");

        VK_CHECK(vkCreateDebugReportCallbackEXT(m_instance, &createInfo, nullptr, &m_callback));
    }


}

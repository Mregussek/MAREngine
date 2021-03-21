
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {

    template<typename TVkProperties>
    static void fillNeeded(std::vector<const char*>& toEnable, const std::vector<const char*>& propertiesToLookFor);

    template<typename TVkProperties>
    static bool pushIfAvailable(std::vector<const char*>& toEnable, const std::vector<TVkProperties>& allProperties, const char* property);

    template<typename TVkProperties>
    static void displayInfoAbout(const std::vector<TVkProperties>& allProperties, const char* message);

    template<typename T>
    struct is_extension_properties {
        static const bool value{ false };
    };

    template<>
    struct is_extension_properties<VkExtensionProperties> {
        static const bool value{ true };
    };

    template<typename T>
    struct is_layer_properties {
        static const bool value{ false };
    };

    template<>
    struct is_layer_properties<VkLayerProperties> {
        static const bool value{ true };
    };



    void ContextVulkan::createInstance() {
        VK_CHECK(volkInitialize());
        const uint32_t apiVersion{ getApiVersion() };

        std::vector<const char*> layersToEnable;
        const std::vector<const char*> layersToLookFor{ 
            "VK_LAYER_KHRONOS_validation"
        };
        fillNeeded<VkLayerProperties>(layersToEnable, layersToLookFor);

        std::vector<const char*> extensionToEnable;
        const std::vector<const char*> extensionsToLookFor{
             VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_REPORT_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME
        };
        fillNeeded<VkExtensionProperties>(extensionToEnable, extensionsToLookFor);

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

        volkLoadInstance(m_instance);

        registerDebugCallback();
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

    void ContextVulkan::closeInstance() const {
        auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);

        vkDestroyInstance(m_instance, nullptr);
    }


    template<typename TVkProperties>
    void fillNeeded(std::vector<const char*>& toEnable, const std::vector<const char*>& propertiesToLookFor) {
        uint32_t count{ 0 };
        std::vector<TVkProperties> properties;
    
        if constexpr (is_extension_properties<TVkProperties>::value) {
            VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr));
            properties.resize(count);
            VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data()));
            //displayInfoAbout(properties, "ExtensionProperties:");
        }
        else if constexpr (is_layer_properties<TVkProperties>::value) {
            VK_CHECK(vkEnumerateInstanceLayerProperties(&count, nullptr));
            properties.resize(count);
            VK_CHECK(vkEnumerateInstanceLayerProperties(&count, properties.data()));
            //displayInfoAbout(properties, "LayerProperties:")
        }
        else {
            static_assert(true, "No available option!");
        }

        for (const char* p : propertiesToLookFor) {
            pushIfAvailable(toEnable, properties, p);
        }
    }

    template<typename TVkProperties>
    bool pushIfAvailable(std::vector<const char*>& toEnable, const std::vector<TVkProperties>& allProperties, const char* propertyToLookFor) {
        const auto isAvailable = [propertyToLookFor](const TVkProperties property) {
            if constexpr (is_extension_properties<TVkProperties>::value) {
                return std::strcmp(property.extensionName, propertyToLookFor) == 0;
            }
            else if constexpr (is_layer_properties<TVkProperties>::value) {
                return std::strcmp(property.layerName, propertyToLookFor) == 0;
            }
            else {
                static_assert(true, "Called not known property!");
                return false;
            }
        };

        if (std::find_if(allProperties.begin(), allProperties.end(), isAvailable) != allProperties.end()) {
            toEnable.emplace_back(propertyToLookFor);
            return true;
        }

        return false;
    }

    template<typename TVkProperties>
    void displayInfoAbout(const std::vector<TVkProperties>& allProperties, const char* message) {
        std::cout << message << '\n';

        for (const TVkProperties& p : allProperties) {
            if constexpr (is_extension_properties<TVkProperties>::value) {
                std::cout << p.extensionName << ", ";
            }
            else if constexpr (is_layer_properties<TVkProperties>::value) {
                std::cout << p.layerName << ", ";
            }
            else {
                static_assert(true, "Called not known property!");
            }
        }

        std::cout << std::endl;
    }


}

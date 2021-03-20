
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {

    static bool supportPresentation(VkPhysicalDevice physicalDevice, uint32_t familyIndex);
    static uint32_t getGraphicsQueueFamily(VkPhysicalDevice physicalDevice);
    static VkPhysicalDevice findSuitablePhysicalDevice(const std::vector<VkPhysicalDevice>& physicalDevices);
    static const std::pair<VkQueueFamilyProperties, uint32_t> getGraphicsQueueFamilyWithProperties(VkPhysicalDevice physicalDevice);

    static void displayInfoAboutPhysicalDevice(VkPhysicalDevice physicalDevice, const char* message);
    static void displayInfoAboutQueueFamilyIndex(const VkQueueFlags& queueFlags);



    void ContextVulkan::createPhysicalDevice() {
        uint32_t physicalDeviceCount{ 0 };
        VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr));

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, physicalDevices.data()));

        m_physicalDevice = findSuitablePhysicalDevice(physicalDevices);

        auto [m_familyQueueProperties, m_familyIndex] = getGraphicsQueueFamilyWithProperties(m_physicalDevice);

        uint32_t layerCount{ 0 };
        VK_CHECK(vkEnumerateDeviceLayerProperties(m_physicalDevice, &layerCount, nullptr));

        std::vector<VkLayerProperties> availableLayers(layerCount);
        VK_CHECK(vkEnumerateDeviceLayerProperties(m_physicalDevice, &layerCount, availableLayers.data()));

        vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_memoryProperties);

        //displayInfoAboutAvailableLayers(availableLayers, "Physical Device available layers: \n");
        //const bool isAvailableKHRONOSValidation = ContextVulkan::pushLayerIfAvailable(layersToEnable, availableLayers, "VK_LAYER_KHRONOS_validation");

        //displayInfoAboutPhysicalDevice(m_physicalDevice, "Main Physical Device");
        //displayInfoAboutQueueFamilyIndex(m_familyQueueProperties.queueFlags);
    }


    VkPhysicalDevice findSuitablePhysicalDevice(const std::vector<VkPhysicalDevice>& physicalDevices) {
        const auto isSuitable = [](const VkPhysicalDevice physicalDevice) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physicalDevice, &properties);
            // properties.limits ---> VkPhysicalDeviceLimits (check this for some parameters)

            //VkPhysicalDeviceFeatures deviceFeatures;  // features can be also checked
            //vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

            const uint32_t familyIndex = getGraphicsQueueFamily(physicalDevice);

            if (familyIndex == VK_QUEUE_FAMILY_IGNORED) {
                return false;
            }
            if (!supportPresentation(physicalDevice, familyIndex)) {
                return false;
            }

            return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        };

        const auto physicalDevProposal = std::find_if(physicalDevices.cbegin(), physicalDevices.cend(), isSuitable);

        if (physicalDevProposal != physicalDevices.cend()) {
            return *physicalDevProposal;
        }
        else if (physicalDevices.size() > 0) {
            return physicalDevices[0];
        }

        return { VK_NULL_HANDLE };
    }

    bool supportPresentation(VkPhysicalDevice physicalDevice, uint32_t familyIndex) {
        if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
            return vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, familyIndex);
        }
        else {
            return false;
        }
    }

    uint32_t getGraphicsQueueFamily(VkPhysicalDevice physicalDevice) {
        uint32_t queuesCount{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, nullptr);

        std::vector<VkQueueFamilyProperties> queues(queuesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, queues.data());

        for (uint32_t i = 0; i < queuesCount; i++) {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                return i;
            }
        }

        return VK_QUEUE_FAMILY_IGNORED;
    }

    const std::pair<VkQueueFamilyProperties, uint32_t> getGraphicsQueueFamilyWithProperties(VkPhysicalDevice physicalDevice) {
        uint32_t queuesCount{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, nullptr);

        std::vector<VkQueueFamilyProperties> queues(queuesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, queues.data());

        for (uint32_t i = 0; i < queuesCount; i++) {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                return { queues[i], i };
            }
        }

        return { { VK_NULL_HANDLE}, VK_QUEUE_FAMILY_IGNORED };
    }



    void displayInfoAboutPhysicalDevice(VkPhysicalDevice physicalDevice, const char* message) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        const std::string apiVersion =
            std::to_string(VK_VERSION_MAJOR(properties.apiVersion)) + "." +
            std::to_string(VK_VERSION_MINOR(properties.apiVersion)) + "." +
            std::to_string(VK_VERSION_PATCH(properties.apiVersion));

        std::cout << message << " picked: " << properties.deviceName << "\n" <<
            "Vulkan Version Available: " << apiVersion << "\n" <<
            "Driver Version: " << properties.driverVersion << "\n" <<
            "Vendor ID: " << properties.vendorID << "\n" <<
            "Device ID: " << properties.deviceID << "\n" <<
            "Device Type: " << properties.deviceType << "\n";
    }

    void displayInfoAboutQueueFamilyIndex(const VkQueueFlags& queueFlags) {
        std::cout << "Queue Family Index info: \n";

        if (queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            std::cout << "- support graphics operations such as drawing points, lines, and triangles \n";
        }
        if (queueFlags & VK_QUEUE_COMPUTE_BIT) {
            std::cout << "- support compute operations such as dispatching compute shaders \n";
        }
        if (queueFlags & VK_QUEUE_TRANSFER_BIT) {
            std::cout << "- support transfer operations such as copying buffer and image contents \n";
        }
        if (queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            std::cout << "- support memory binding operations used to update sparse resources \n";
        }
    }


}

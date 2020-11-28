

#include "PhysicalDevVulkan.h"
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


    void PhysicalDevVulkan::create(VkInstance instance) {
        m_physicalDevice = selectPhysicalDevice(instance);
        m_familyIndex = getGraphicsQueueFamily(m_physicalDevice);
    }

    bool PhysicalDevVulkan::supportPresentation(VkPhysicalDevice physicalDevice, uint32_t familyIndex) {
        if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
            return vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, familyIndex);
        }
        else {
            return false;
        }
    }

    uint32_t PhysicalDevVulkan::getGraphicsQueueFamily(VkPhysicalDevice physicalDevice) {
        uint32_t queuesCount{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, nullptr);

        std::vector<VkQueueFamilyProperties> queues(queuesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, queues.data());

        for (uint32_t i = 0; i < queuesCount; i++) {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { return i; }
        }

        return VK_QUEUE_FAMILY_IGNORED;
    }

    VkPhysicalDevice PhysicalDevVulkan::selectPhysicalDevice(VkInstance instance) {
        uint32_t physicalDeviceCount{ 0 };
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()));

        auto pickPhysicalDevice = [&physicalDevices, this]()->VkPhysicalDevice {
            const auto isSuitable = [this](const VkPhysicalDevice physicalDevice) {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(physicalDevice, &properties);
                // properties.limits ---> VkPhysicalDeviceLimits (check this for some parameters)

                //VkPhysicalDeviceFeatures deviceFeatures;  // features can be also checked
                //vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

                const auto familyIndex = getGraphicsQueueFamily(physicalDevice);

                if (familyIndex == VK_QUEUE_FAMILY_IGNORED) { return false; }
                if (!supportPresentation(physicalDevice, familyIndex)) { return false; };

                return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
            };

            const auto it = std::find_if(physicalDevices.cbegin(), physicalDevices.cend(), isSuitable);

            if (it != physicalDevices.end()) {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(*it, &properties);
                std::cout << "Picking: " << properties.deviceName << "\n";
                return *it;
            }
            else if (physicalDevices.size() > 0) {
                VkPhysicalDeviceProperties fallbackProperties;
                vkGetPhysicalDeviceProperties(physicalDevices[0], &fallbackProperties);
                std::cout << "Picking fallback: " << fallbackProperties.deviceName << "\n";
                return physicalDevices[0];
            }

            std::cout << "No physical devices available!\n";
            return VkPhysicalDevice{ VK_NULL_HANDLE };
        };

        return pickPhysicalDevice();
    }

    void PhysicalDevVulkan::fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& extensionsToEnable) {
        uint32_t extensionCount{ 0 };
        VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr));

        std::vector<VkExtensionProperties> extensionProperties(extensionCount);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensionProperties.data()));

        const bool isAvailableSwapchain = ContextVulkan::pushExtensionIfAvailable(extensionsToEnable, extensionProperties, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }


}

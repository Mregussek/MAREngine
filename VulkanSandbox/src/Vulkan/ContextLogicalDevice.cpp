
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {

    static void fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice,
                                           std::vector<const char*>& extensionsToEnable,
                                           const std::vector<const char*>& extensionsToLookFor);
    static bool pushExtensionIfAvailable(std::vector<const char*>& toEnable,
                                         std::vector<VkExtensionProperties>& allProperties,
                                         const char* property);;



    void ContextVulkan::createLogicalDevice() {
        constexpr size_t index{ 0 };
        const std::array<float, 1> queuePriorities{ 1.f };

        std::array<VkDeviceQueueCreateInfo, 1> deviceQueueInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        deviceQueueInfo[index].queueFamilyIndex = m_familyIndex;
        deviceQueueInfo[index].queueCount = queuePriorities.size();
        deviceQueueInfo[index].pQueuePriorities = queuePriorities.data();
        deviceQueueInfo[index].pNext = nullptr;

        std::vector<const char*> extensionsToEnable;
        const std::vector<const char*> extensionsToLookFor{
            VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME
        };
        fillDeviceNeededExtensions(m_physicalDevice, extensionsToEnable, extensionsToLookFor);

        VkPhysicalDeviceFeatures features{};
        features.vertexPipelineStoresAndAtomics = VK_TRUE; // we aren't using this yet

        VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
        createInfo.queueCreateInfoCount = deviceQueueInfo.size();
        createInfo.pQueueCreateInfos = deviceQueueInfo.data();
        createInfo.enabledExtensionCount = extensionsToEnable.size();
        createInfo.ppEnabledExtensionNames = extensionsToEnable.data();
        createInfo.pEnabledFeatures = &features;

        VK_CHECK(vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device));
    }

    void ContextVulkan::closeLogicalDevice() const {
        vkDestroyDevice(m_device, nullptr);
    }



    void fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice,
                                    std::vector<const char*>& extensionsToEnable,
                                    const std::vector<const char*>& extensionsToLookFor) {
        uint32_t extensionCount{ 0 };
        VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr));

        std::vector<VkExtensionProperties> extensionProperties(extensionCount);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensionProperties.data()));

        for (const char* p : extensionsToLookFor) {
            pushExtensionIfAvailable(extensionsToEnable, extensionProperties, p);
        }
    }

    bool pushExtensionIfAvailable(std::vector<const char*>& extensionsToEnable,
                                  std::vector<VkExtensionProperties>& allExtensions,
                                  const char* extensionProperty) {
        const auto availableExtension = [extensionProperty](const VkExtensionProperties extension) {
            return std::strcmp(extension.extensionName, extensionProperty) == 0;
        };

        if (std::find_if(allExtensions.cbegin(), allExtensions.cend(), availableExtension) != allExtensions.cend()) {
            extensionsToEnable.emplace_back(extensionProperty);
            return true;
        }

        return false;
    }


}

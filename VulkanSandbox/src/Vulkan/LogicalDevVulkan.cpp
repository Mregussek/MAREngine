

#include "LogicalDevVulkan.h"
#include "PhysicalDevVulkan.h"
#include "ExtensionFiller.h"
#include "../../VulkanLogging.h"


namespace mar {


    void LogicalDevVulkan::create() {
        constexpr size_t index{ 0 };
        const std::array<float, 1> queuePriorities{ 1.f };

        std::array<VkDeviceQueueCreateInfo, 1> deviceQueueInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        deviceQueueInfo[index].queueFamilyIndex = PhysicalDevVulkan::Instance()->getFamilyIndex();
        deviceQueueInfo[index].queueCount = queuePriorities.size();
        deviceQueueInfo[index].pQueuePriorities = queuePriorities.data();
        deviceQueueInfo[index].pNext = nullptr;

        std::vector<const char*> extensionsToEnable;
        fillDeviceNeededExtensions(PhysicalDevVulkan::Instance()->getPhyDev(), extensionsToEnable);

        VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
        createInfo.queueCreateInfoCount = deviceQueueInfo.size();
        createInfo.pQueueCreateInfos = deviceQueueInfo.data();
        createInfo.enabledExtensionCount = extensionsToEnable.size();
        createInfo.ppEnabledExtensionNames = extensionsToEnable.data();

        VK_CHECK( vkCreateDevice(PhysicalDevVulkan::Instance()->getPhyDev(), &createInfo, nullptr, &m_device) );
    }

    void LogicalDevVulkan::close() {
        vkDestroyDevice(m_device, nullptr);
    }

    void LogicalDevVulkan::fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& extensionsToEnable) {
        uint32_t extensionCount{ 0 };
        VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr));

        std::vector<VkExtensionProperties> extensionProperties(extensionCount);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensionProperties.data()));

        const bool isAvailableSwapchain = pushExtensionIfAvailable(extensionsToEnable, extensionProperties, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    }


}

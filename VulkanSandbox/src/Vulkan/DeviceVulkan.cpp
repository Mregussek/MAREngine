

#include "DeviceVulkan.h"
#include "PhysicalDevVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


    void DeviceVulkan::create(VkPhysicalDevice physicalDevice, uint32_t familyIndex) {
        float queuePriority{ 1.f };

        VkDeviceQueueCreateInfo queueInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
        queueInfo.queueFamilyIndex = familyIndex;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;

        std::vector<const char*> extensionsToEnable;
        PhysicalDevVulkan::fillDeviceNeededExtensions(physicalDevice, extensionsToEnable);

        VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
        createInfo.queueCreateInfoCount = 1;
        createInfo.pQueueCreateInfos = &queueInfo;
        createInfo.enabledExtensionCount = extensionsToEnable.size();
        createInfo.ppEnabledExtensionNames = extensionsToEnable.data();

        VK_CHECK( vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_device) );
    }

    void DeviceVulkan::close() {
        vkDestroyDevice(m_device, nullptr);
    }


}



#include "PhysicalDevVulkan.h"
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {

    PhysicalDevVulkan* PhysicalDevVulkan::s_instance{ nullptr };

    PhysicalDevVulkan* PhysicalDevVulkan::Instance() {
        return s_instance;
    }


    void PhysicalDevVulkan::create() {
        m_physicalDevice = selectPhysicalDevice();
        m_familyIndex = getGraphicsQueueFamily(m_physicalDevice, true);

        std::vector<const char*> layersToEnable;
        checkDeviceLayerProperties(layersToEnable);

        s_instance = this;
    }

    const VkPhysicalDevice& PhysicalDevVulkan::getPhyDev() const {
        return m_physicalDevice;
    }

    const uint32_t& PhysicalDevVulkan::getFamilyIndex() const {
        return m_familyIndex;
    }

    bool PhysicalDevVulkan::supportPresentation(VkPhysicalDevice physicalDevice, uint32_t familyIndex) const {
        if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
            return vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, familyIndex);
        }
        else {
            return false;
        }
    }

    uint32_t PhysicalDevVulkan::getGraphicsQueueFamily(VkPhysicalDevice physicalDevice, bool displayInfo) const {
        uint32_t queuesCount{ 0 };
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, nullptr);

        std::vector<VkQueueFamilyProperties> queues(queuesCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, queues.data());

        for (uint32_t i = 0; i < queuesCount; i++) {
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { 
                if (displayInfo) { displayInfoAboutQueueFamilyIndex(queues[i].queueFlags); }
                return i;
            }
        }

        return VK_QUEUE_FAMILY_IGNORED;
    }

    VkPhysicalDevice PhysicalDevVulkan::selectPhysicalDevice() const {
        const auto instance = ContextVulkan::Instance()->get();

        uint32_t physicalDeviceCount{ 0 };
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr));

        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        VK_CHECK(vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()));

        const auto isSuitable = [this](const VkPhysicalDevice physicalDevice) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physicalDevice, &properties);
            // properties.limits ---> VkPhysicalDeviceLimits (check this for some parameters)

            //VkPhysicalDeviceFeatures deviceFeatures;  // features can be also checked
            //vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

            const auto familyIndex = getGraphicsQueueFamily(physicalDevice, false);

            if (familyIndex == VK_QUEUE_FAMILY_IGNORED) { return false; }
            if (!supportPresentation(physicalDevice, familyIndex)) { return false; };

            return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        };

        const auto physicalDevProposal = std::find_if(physicalDevices.cbegin(), physicalDevices.cend(), isSuitable);

        if (physicalDevProposal != physicalDevices.end()) {
            displayInfoAboutPhysicalDevice(*physicalDevProposal, "Main Physical Device");
            return *physicalDevProposal;
        }
        else if (physicalDevices.size() > 0) {
            displayInfoAboutPhysicalDevice(physicalDevices[0], "Fallback Physical Device");
            return physicalDevices[0];
        }

        return { VK_NULL_HANDLE };
    }

    void PhysicalDevVulkan::checkDeviceLayerProperties(std::vector<const char*>& layersToEnable) const {
        uint32_t layerCount{ 0 };
        VK_CHECK( vkEnumerateDeviceLayerProperties(m_physicalDevice, &layerCount, nullptr) );

        std::vector<VkLayerProperties> availableLayers(layerCount);
        VK_CHECK( vkEnumerateDeviceLayerProperties(m_physicalDevice, &layerCount, availableLayers.data()) );

        std::cout << "Physical Device available layers: \n";
        std::for_each(availableLayers.begin(), availableLayers.end(), [](const VkLayerProperties& property) {
            std::cout << property.layerName << " ";
        });
        std::cout << "\n";

        //const bool isAvailableKHRONOSValidation = ContextVulkan::pushLayerIfAvailable(layersToEnable, availableLayers, "VK_LAYER_KHRONOS_validation");
    }

    void PhysicalDevVulkan::displayInfoAboutPhysicalDevice(VkPhysicalDevice physicalDevice, const char* message) const {
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

    void PhysicalDevVulkan::displayInfoAboutQueueFamilyIndex(const VkQueueFlags& queueFlags) const {
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

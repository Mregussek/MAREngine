

#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {
    

    void ContextVulkan::initialize() {
        createInstance();
        createPhysicalDevice();
        createLogicalDevice();
    }

    void ContextVulkan::terminate() const {
        vkDestroyDevice(m_device, nullptr);

        auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT(m_instance, m_callback, nullptr);

        vkDestroyInstance(m_instance, nullptr);
    }

    void ContextVulkan::endPendingJobs() const {
        VK_CHECK(vkDeviceWaitIdle(m_device));
    }

    uint32_t ContextVulkan::getApiVersion() {
        uint32_t apiVersion{ 0 };
        VK_CHECK( vkEnumerateInstanceVersion(&apiVersion) );

        //std::cout << "Supported version: " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) << "." << VK_VERSION_PATCH(apiVersion) << "\n";

        return apiVersion;
    }


}

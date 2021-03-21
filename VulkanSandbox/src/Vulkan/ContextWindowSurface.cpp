
#include "ContextVulkan.h"
#include "../Window/Window.h"
#include "../../VulkanLogging.h"


namespace mar {

    static VkFormat getSurfaceFormat(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
    static VkPresentModeKHR getPresentMode(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);


	void ContextVulkan::createWindowSurface() {
        if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
            VkWin32SurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
            createInfo.hinstance = GetModuleHandle(nullptr);
            createInfo.hwnd = glfwGetWin32Window(m_pWindow->getWindow());

            VK_CHECK( vkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_surface) );
        }
        else {
            static_assert(true, "Unsupported platform!\n");
            return;
        }

        VkBool32 presentSupported{ VK_FALSE };
        VK_CHECK( vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, m_familyIndex, m_surface, &presentSupported) );

        if (!presentSupported) {
            static_assert(true, "Presenting is not supported!\n");
            return;
        }

        m_format = getSurfaceFormat(m_physicalDevice, m_surface);
        m_presentMode = getPresentMode(m_physicalDevice, m_surface);

        updateWindowSurface();
	}

    void ContextVulkan::updateWindowSurface() {
        VK_CHECK( vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &m_surfaceCaps) );
    }

    void ContextVulkan::closeWindowSurface() const {
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    }



    VkFormat getSurfaceFormat(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface) {
        uint32_t formatCount{ 0 };
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr));

        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data()));

        if (formatCount == 1 && formats[0].format == VK_FORMAT_UNDEFINED) { return VK_FORMAT_R8G8B8A8_UNORM; }

        /*
        const auto it = std::find_if(formats.cbegin(), formats.cend(), [](const VkSurfaceFormatKHR surfaceFormat) {
            return surfaceFormat.format == VK_FORMAT_A2R10G10B10_UNORM_PACK32 || surfaceFormat.format == VK_FORMAT_A2B10G10R10_UNORM_PACK32;
        });

        if (it != formats.cend()) { return (*it).format; }
        */
        {
            const auto it = std::find_if(formats.cbegin(), formats.cend(), [](const VkSurfaceFormatKHR surfaceFormat) {
                return surfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM || surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM;
            });

            if (it != formats.cend()) { return (*it).format; }
        }

        return formats[0].format;
    }

    VkPresentModeKHR getPresentMode(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface) {
        uint32_t presentModeCount{ 0 };
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()));

        const auto it = std::find_if(presentModes.cbegin(), presentModes.cend(), [](const VkPresentModeKHR mode) {
            return mode == VK_PRESENT_MODE_MAILBOX_KHR;
        });

        if (it != presentModes.cend()) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }


}


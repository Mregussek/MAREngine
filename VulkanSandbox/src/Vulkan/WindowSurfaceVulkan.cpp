

#include "WindowSurfaceVulkan.h"
#include "../Window/Window.h"
#include "ContextVulkan.h"
#include "PhysicalDevVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {



    void WindowSurfaceVulkan::create() {
        if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
            VkWin32SurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
            createInfo.hinstance = GetModuleHandle(nullptr);
            createInfo.hwnd = glfwGetWin32Window(Window::Instance()->getWindow());
            
            VK_CHECK( vkCreateWin32SurfaceKHR(ContextVulkan::Instance()->get() , &createInfo, nullptr, &m_surface) );
        }
        else {
            std::cout << "MAR_ENGINE ERROR: Unsupported platform!\n";
        }

        VkBool32 presentSupported{ VK_FALSE };
        VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevVulkan::Instance()->getPhyDev(), PhysicalDevVulkan::Instance()->getFamilyIndex(), m_surface, &presentSupported));

        if (!presentSupported) {
            std::cout << "MAR_ENGINE ERROR: Presenting is not supported!\n";
            return;
        }

        m_format = getSurfaceFormat();
        m_presentMode = getPresentMode();
    }

    void WindowSurfaceVulkan::close() const {
        vkDestroySurfaceKHR(ContextVulkan::Instance()->get(), m_surface, nullptr);
    }

    VkSurfaceKHR WindowSurfaceVulkan::getSurface() const {
        return m_surface;
    }

    VkPresentModeKHR WindowSurfaceVulkan::getSwapchainPresentMode() const {
        return m_presentMode;
    }

    VkFormat WindowSurfaceVulkan::getSwapchainFormat() const {
        return m_format;
    }

    VkFormat WindowSurfaceVulkan::getSurfaceFormat() const {
        const auto& physicalDevice = PhysicalDevVulkan::Instance()->getPhyDev();

        uint32_t formatCount{ 0 };
        VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, nullptr) );

        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, formats.data()) );

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

    VkPresentModeKHR WindowSurfaceVulkan::getPresentMode() const {
        const auto& physicalDevice = PhysicalDevVulkan::Instance()->getPhyDev();
        
        uint32_t presentModeCount{ 0 };
        VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, nullptr) );

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, presentModes.data()) );

        const auto it = std::find_if(presentModes.cbegin(), presentModes.cend(), [](const VkPresentModeKHR mode) {
            return mode == VK_PRESENT_MODE_MAILBOX_KHR;
        });

        if (it != presentModes.cend()) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }


}

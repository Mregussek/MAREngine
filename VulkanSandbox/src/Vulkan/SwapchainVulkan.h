

#ifndef MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


    struct SwapchainVulkan {

        VkSwapchainKHR swapchain;

        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebuffer> framebuffers;

        VkExtent2D extent;

        SwapchainVulkan() = default;
        SwapchainVulkan(VkExtent2D swapchainSize);

        void create(VkDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCaps, VkPresentModeKHR presentMode, VkFormat format, const uint32_t* familyIndex);
        void close(VkDevice device);

        void fillImageViewsAndFramebuffers(VkDevice device, VkRenderPass renderPass, VkFormat format, VkExtent2D extent);

    private:

        void close(VkDevice device, VkSwapchainKHR swapchain);

    };


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H

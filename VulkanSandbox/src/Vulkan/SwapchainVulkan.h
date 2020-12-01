

#ifndef MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


    class SwapchainVulkan {
    public:

        VkSwapchainKHR swapchain{ VK_NULL_HANDLE };
        VkSwapchainKHR oldSwapchain{ VK_NULL_HANDLE };

        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebuffer> framebuffers;

        VkExtent2D extent;

        uint32_t imageCount{ 0 };


        SwapchainVulkan() = default;
        SwapchainVulkan(VkExtent2D swapchainSize);

        void create(VkDevice device, VkSurfaceKHR surface, VkPresentModeKHR presentMode, VkFormat format);
        void close(VkDevice device);

        void fillImageViewsAndFramebuffers(VkDevice device, VkRenderPass renderPass, VkFormat format);

        void resizeIfNecessary(VkDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCaps, VkPresentModeKHR presentMode, VkFormat format, VkRenderPass renderPass);

    private:

        VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);
        VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, VkExtent2D extent);

    };


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H



#ifndef MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

    class WindowSurfaceVulkan;


    class SwapchainVulkan {
    public:

        VkSwapchainKHR swapchain{ VK_NULL_HANDLE };
        VkSwapchainKHR oldSwapchain{ VK_NULL_HANDLE };

        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebuffer> framebuffers;

        uint32_t imageCount{ 0 };
        VkExtent2D m_extent;


        void create(const WindowSurfaceVulkan& windowSurface, VkRenderPass renderPass);
        void close();

        void resizeIfNecessary(const WindowSurfaceVulkan& windowSurface, VkRenderPass renderPass);

        uint32_t getWidth() const;
        uint32_t getHeight() const;

    private:

        void fillImageViewsAndFramebuffers(VkRenderPass renderPass, VkFormat format);

        VkImageView createImageView(VkDevice device, VkImage image, VkFormat format);
        VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView);

    };


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_SWAPCHAIN_VULKAN_H

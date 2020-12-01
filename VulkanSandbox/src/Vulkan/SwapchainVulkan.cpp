

#include "SwapchainVulkan.h"
#include "PhysicalDevVulkan.h"
#include "LogicalDevVulkan.h"
#include "WindowSurfaceVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


    void SwapchainVulkan::create(const WindowSurfaceVulkan& windowSurface, VkRenderPass renderPass) {
        const auto surface = windowSurface.getSurface();
        const auto presentMode = windowSurface.getSwapchainPresentMode();
        const auto format = windowSurface.getSwapchainFormat();
        const auto& surfaceCaps = windowSurface.getSurfaceCaps();

        m_extent = surfaceCaps.currentExtent;

        const auto surfaceComposite = [&surfaceCaps]()->VkCompositeAlphaFlagBitsKHR {
            if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR };
            }
            else if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR };
            }
            else if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR };
            }
            else {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR };
            }
        }();

        VkSwapchainCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
        createInfo.surface = surface;
        createInfo.minImageCount = std::max(2u, surfaceCaps.minImageCount);
        createInfo.imageFormat = format;
        createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        createInfo.imageExtent = m_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.queueFamilyIndexCount = 1;
        createInfo.pQueueFamilyIndices = &PhysicalDevVulkan::Instance()->getFamilyIndex();
        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        createInfo.compositeAlpha = surfaceComposite;
        createInfo.presentMode = presentMode;
        createInfo.oldSwapchain = oldSwapchain;

        VK_CHECK( vkCreateSwapchainKHR(LogicalDevVulkan::Instance()->getDev(), &createInfo, nullptr, &swapchain) );

        fillImageViewsAndFramebuffers(renderPass, format);
    }

    void SwapchainVulkan::close() {
        const auto& device = LogicalDevVulkan::Instance()->getDev();

        std::for_each(imageViews.cbegin(), imageViews.cend(), [&device](const VkImageView& imageView) {
            vkDestroyImageView(device, imageView, nullptr);
        });

        std::for_each(framebuffers.cbegin(), framebuffers.cend(), [&device](const VkFramebuffer& framebuffer) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        });

        vkDestroySwapchainKHR(device, swapchain, nullptr);

        images.clear();
        imageViews.clear();
        framebuffers.clear();
    }

    void SwapchainVulkan::resizeIfNecessary(const WindowSurfaceVulkan& windowSurface, VkRenderPass renderPass) {
        const auto format = windowSurface.getSwapchainFormat();

        SwapchainVulkan swapchainToReplace;
        swapchainToReplace.oldSwapchain = this->swapchain;
        swapchainToReplace.create(windowSurface, renderPass);
        swapchainToReplace.fillImageViewsAndFramebuffers(renderPass, format);

        LogicalDevVulkan::Instance()->endPendingJobs();

        close();

        *this = swapchainToReplace;
    }

    uint32_t SwapchainVulkan::getWidth() const {
        return m_extent.width;
    }

    uint32_t SwapchainVulkan::getHeight() const {
        return m_extent.height;
    }

    void SwapchainVulkan::fillImageViewsAndFramebuffers(VkRenderPass renderPass, VkFormat format) {
        const auto& device = LogicalDevVulkan::Instance()->getDev();

        VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr));

        images.resize(imageCount);
        imageViews.resize(imageCount);
        framebuffers.resize(imageCount);

        VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data()));

        for (uint32_t i = 0; i < imageCount; i++) {
            imageViews[i] = createImageView(device, images[i], format);
        }

        for (uint32_t i = 0; i < imageCount; i++) {
            framebuffers[i] = createFramebuffer(device, renderPass, imageViews[i]);
        }
    }

    VkImageView SwapchainVulkan::createImageView(VkDevice device, VkImage image, VkFormat format) {
        VkImageViewCreateInfo createInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
        createInfo.image = image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.layerCount = 1;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        VkImageView view{ 0 };
        VK_CHECK(vkCreateImageView(device, &createInfo, nullptr, &view));

        return view;
    }

    VkFramebuffer SwapchainVulkan::createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView) {
        VkFramebufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
        createInfo.renderPass = renderPass;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &imageView;
        createInfo.width = m_extent.width;
        createInfo.height = m_extent.height;
        createInfo.layers = 1;

        VkFramebuffer framebuffer{ 0 };
        VK_CHECK(vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffer));

        return framebuffer;
    }


}

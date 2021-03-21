
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {

    static VkImageView createImageView(const VkDevice& device, VkImage image, VkFormat format);
    static VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, VkExtent2D extent);
    static void fillSwapchain(const VkRenderPass& renderPass, VkFormat format);



	void ContextVulkan::createSwapchain() {
        m_extent = m_surfaceCaps.currentExtent;

        const auto surfaceComposite = [this]()->VkCompositeAlphaFlagBitsKHR {
            if (m_surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR };
            }
            else if (m_surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR };
            }
            else if (m_surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR };
            }
            else {
                return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR };
            }
        }();

        VkSwapchainCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
        createInfo.surface = m_surface;
        createInfo.minImageCount = std::max(2u, m_surfaceCaps.minImageCount);
        createInfo.imageFormat = m_format;
        createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        createInfo.imageExtent = m_extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.queueFamilyIndexCount = 1;
        createInfo.pQueueFamilyIndices = &m_familyIndex;
        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        createInfo.compositeAlpha = surfaceComposite;
        createInfo.presentMode = m_presentMode;
        createInfo.oldSwapchain = m_oldSwapchain;

        VK_CHECK( vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain) );

        fillSwapchain();
	}

    void ContextVulkan::fillSwapchain() {
        VK_CHECK(vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_imageCount, nullptr));

        m_images.resize(m_imageCount);
        m_imageViews.resize(m_imageCount);
        m_framebuffers.resize(m_imageCount);

        VK_CHECK(vkGetSwapchainImagesKHR(m_device, m_swapchain, &m_imageCount, m_images.data()));

        for (uint32_t i = 0; i < m_imageCount; i++) {
            m_imageViews[i] = createImageView(m_device, m_images[i], m_format);
        }

        for (uint32_t i = 0; i < m_imageCount; i++) {
            m_framebuffers[i] = createFramebuffer(m_device, m_renderPass, m_imageViews[i], m_extent);
        }
    }

    void ContextVulkan::resizeSwapchain() {
        m_oldSwapchain = m_swapchain;

        closeSwapchain();
        createSwapchain();
        endPendingJobs();
    }

	void ContextVulkan::closeSwapchain() {
        for (VkImageView& imageView : m_imageViews) {
            vkDestroyImageView(m_device, imageView, nullptr);
        }

        for (VkFramebuffer& framebuffer : m_framebuffers) {
            vkDestroyImageView(m_device, framebuffer, nullptr);
        }

        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

        m_images.clear();
        m_imageViews.clear();
        m_framebuffers.clear();
	}



    VkImageView createImageView(const VkDevice& device, VkImage image, VkFormat format) {
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

    VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, VkExtent2D extent) {
        VkFramebufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
        createInfo.renderPass = renderPass;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &imageView;
        createInfo.width = extent.width;
        createInfo.height = extent.height;
        createInfo.layers = 1;

        VkFramebuffer framebuffer{ 0 };
        VK_CHECK(vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffer));

        return framebuffer;
    }

}


#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {

    static VkImageView createImageView(const VkDevice& device, VkImage image, VkFormat format);
    static VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, VkExtent2D extent);



	void ContextVulkan::Swapchain::create(ContextVulkan* pContext) {
        extent = pContext->m_surfaceCaps.currentExtent;

        const auto surfaceComposite = [&surfaceCaps = pContext->m_surfaceCaps]()->VkCompositeAlphaFlagBitsKHR {
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
        createInfo.surface = pContext->m_surface;
        createInfo.minImageCount = std::max(2u, pContext->m_surfaceCaps.minImageCount);
        createInfo.imageFormat = pContext->m_format;
        createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        createInfo.queueFamilyIndexCount = 1;
        createInfo.pQueueFamilyIndices = &pContext->m_familyIndex;
        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        createInfo.compositeAlpha = surfaceComposite;
        createInfo.presentMode = pContext->m_presentMode;
        createInfo.oldSwapchain = oldSwapchainKHR;

        VK_CHECK( vkCreateSwapchainKHR(pContext->m_device, &createInfo, nullptr, &swapchainKHR) );

        fill(pContext);
	}

    void ContextVulkan::Swapchain::fill(ContextVulkan* pContext) {
        VK_CHECK(vkGetSwapchainImagesKHR(pContext->m_device, swapchainKHR, &imageCount, nullptr));

        images.resize(imageCount);
        imageViews.resize(imageCount);
        framebuffers.resize(imageCount);

        VK_CHECK(vkGetSwapchainImagesKHR(pContext->m_device, swapchainKHR, &imageCount, images.data()));

        for (uint32_t i = 0; i < imageCount; i++) {
            imageViews[i] = createImageView(pContext->m_device, images[i], pContext->m_format);
        }

        for (uint32_t i = 0; i < imageCount; i++) {
            framebuffers[i] = createFramebuffer(pContext->m_device, pContext->m_renderPass, imageViews[i], extent);
        }
    }

    void ContextVulkan::Swapchain::resize(ContextVulkan* pContext) {
        Swapchain swapchainToReplace;

        swapchainToReplace.oldSwapchainKHR = swapchainKHR;
        swapchainToReplace.create(pContext);
        pContext->endPendingJobs();

        close(pContext);

        *this = swapchainToReplace;
    }

    void ContextVulkan::Swapchain::prepareFrame(ContextVulkan* pContext) {
        const bool windowSizeHasChanged = !(
            extent.width == pContext->m_surfaceCaps.currentExtent.width &&
            extent.height == pContext->m_surfaceCaps.currentExtent.height);

        if (windowSizeHasChanged) {
            resize(pContext);
        }

        VK_CHECK(vkAcquireNextImageKHR(pContext->m_device, swapchainKHR, UINT64_MAX, pContext->m_acquireSemaphore, VK_NULL_HANDLE, &pContext->m_imageIndex));
    }

	void ContextVulkan::Swapchain::close(ContextVulkan* pContext) {
        for (const VkImageView& imageView : imageViews) {
            vkDestroyImageView(pContext->m_device, imageView, nullptr);
        }

        for (const VkFramebuffer& framebuffer : framebuffers) {
            vkDestroyFramebuffer(pContext->m_device, framebuffer, nullptr);
        }

        vkDestroySwapchainKHR(pContext->m_device, swapchainKHR, nullptr);

        images.clear();
        imageViews.clear();
        framebuffers.clear();
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

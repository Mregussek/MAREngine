

#ifndef MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

    class Window;


	class ContextVulkan {
    public:

        static uint32_t getApiVersion();

        void initialize(Window* pWindow);
        void terminate();

        void beginFrame();
        void updateFrame();
        void endFrame();

        void endPendingJobs() const;

        const VkCommandBuffer& getCommandBuffer() const;
        const VkRenderPass& getRenderPass() const;
        const VkViewport& getViewport() const;
        const VkRect2D& getScissor() const;
        const VkDevice& getLogicalDevice() const;
        const VkPhysicalDeviceMemoryProperties& getMemoryProperties() const;

    private:

        void createInstance();
        void registerDebugCallback();
        void closeInstance() const;

        void createPhysicalDevice();

        void createLogicalDevice();
        void closeLogicalDevice() const;

        void createWindowSurface();
        void updateWindowSurface();
        void closeWindowSurface() const;

        void createRenderPass();
        void beginRenderPass();
        void endRenderPass();
        void closeRenderPass() const;

        void createCommandPool();
        void closeCommandPool() const;

        void createCommandBuffer();
        void beginCommandBuffer();
        void endCommandBuffer();

        void createDeviceQueue();
        void closeDeviceQueue() const;


        // Instance members

        VkInstance m_instance{ VK_NULL_HANDLE };
        VkDebugReportCallbackEXT m_callback{ VK_NULL_HANDLE };

        // Physical Device members

        VkPhysicalDevice m_physicalDevice{ VK_NULL_HANDLE };
        VkQueueFamilyProperties m_familyQueueProperties{ VK_NULL_HANDLE };
        VkPhysicalDeviceMemoryProperties m_memoryProperties{ VK_NULL_HANDLE };
        uint32_t m_familyIndex{ 0 };

        // Logical Device members

        VkDevice m_device{ VK_NULL_HANDLE };

        // Window Surface members

        VkSurfaceCapabilitiesKHR m_surfaceCaps{ VK_NULL_HANDLE };
        VkSurfaceKHR m_surface{ VK_NULL_HANDLE };
        VkFormat m_format{ VK_FORMAT_UNDEFINED };
        VkPresentModeKHR m_presentMode{ VK_PRESENT_MODE_FIFO_KHR };

        // RenderPass members

        VkRenderPass m_renderPass{ VK_NULL_HANDLE };
        VkCommandPool m_commandPool{ VK_NULL_HANDLE };
        VkCommandBuffer m_commandBuffer{ VK_NULL_HANDLE };

        // Device Queue members

        VkQueue m_queue{ VK_NULL_HANDLE };
        VkSemaphore m_acquireSemaphore{ VK_NULL_HANDLE };
        VkSemaphore m_releaseSemaphore{ VK_NULL_HANDLE };

        // Swapchain members

        struct Swapchain {

            void create(ContextVulkan* pContext);
            void fill(ContextVulkan* pContext);
            void resize(ContextVulkan* pContext);
            void close(ContextVulkan* pContext);

            VkSwapchainKHR swapchainKHR{ VK_NULL_HANDLE };
            VkSwapchainKHR oldSwapchainKHR{ VK_NULL_HANDLE };

            std::vector<VkImage> images;
            std::vector<VkImageView> imageViews;
            std::vector<VkFramebuffer> framebuffers;

            uint32_t imageCount{ 0 };
            VkExtent2D extent;

        } m_swapchain;

        // Other

        VkViewport m_viewport;
        VkRect2D m_scissor;
        uint32_t m_imageIndex{ 0 };

        Window* m_pWindow{ nullptr };

	};


}

#endif // !MAR_ENGINE_PLATFORM_VULKAN_CONTEXT_VULKAN_H
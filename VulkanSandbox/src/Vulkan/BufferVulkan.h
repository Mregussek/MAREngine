
#ifndef MAR_ENGINE_PLATFORM_VULKAN_BUFFER_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_BUFFER_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


    class BufferVulkan {
    public:

        VkBuffer m_buffer{ VK_NULL_HANDLE };
        VkDeviceMemory m_deviceMemory{ VK_NULL_HANDLE };
        VkDeviceSize m_size{ VK_NULL_HANDLE };
        void* m_data{ nullptr };

        void create(VkDevice device, VkDeviceSize size, VkBufferUsageFlags useFlags);
        void close(VkDevice device);

    private:

        uint32_t getMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t memoryTypeBits, VkMemoryPropertyFlags flags) const;

    };


}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_BUFFER_VULKAN_H
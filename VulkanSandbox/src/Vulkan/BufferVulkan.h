
#ifndef MAR_ENGINE_PLATFORM_VULKAN_BUFFER_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_BUFFER_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

    class ContextVulkan;


    class BufferVulkan {
    public:

        VkBuffer m_buffer{ VK_NULL_HANDLE };
        VkDeviceMemory m_deviceMemory{ VK_NULL_HANDLE };
        VkDeviceSize m_size{ VK_NULL_HANDLE };
        void* m_data{ nullptr };

        void create(ContextVulkan* pContext, VkDeviceSize size, VkBufferUsageFlags useFlags);
        void close();

        template<typename T>
        void update(const std::vector<T>& data) {
            memcpy(m_data, data.data(), data.size() * sizeof(T));
        }

    private:

        uint32_t getMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t memoryTypeBits, VkMemoryPropertyFlags flags) const;


        ContextVulkan* m_pContext{ nullptr };

    };


}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_BUFFER_VULKAN_H
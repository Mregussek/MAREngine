

#include "BufferVulkan.h"
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


	void BufferVulkan::create(ContextVulkan* pContext, VkDeviceSize size, VkBufferUsageFlags useFlags) {
		m_pContext = pContext;
		m_size = size;

		VkBufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		createInfo.size = m_size;
		createInfo.usage = useFlags;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VK_CHECK( vkCreateBuffer(m_pContext->getLogicalDevice(), &createInfo, nullptr, &m_buffer) );
	
		VkMemoryRequirements memoryReqs;
		vkGetBufferMemoryRequirements(m_pContext->getLogicalDevice(), m_buffer, &memoryReqs);

		constexpr VkMemoryPropertyFlags requiredMemoryFlags{ VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT };

		VkMemoryAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
		allocateInfo.allocationSize = memoryReqs.size;
		allocateInfo.memoryTypeIndex = getMemoryTypeIndex(m_pContext->getMemoryProperties(), memoryReqs.memoryTypeBits, requiredMemoryFlags);

		constexpr VkDeviceSize memoryOffset{ VK_NULL_HANDLE };
		constexpr VkMemoryMapFlags memoryMapFlags{ VK_NULL_HANDLE };

		VK_CHECK( vkAllocateMemory(m_pContext->getLogicalDevice(), &allocateInfo, nullptr, &m_deviceMemory) );

		VK_CHECK( vkBindBufferMemory(m_pContext->getLogicalDevice(), m_buffer, m_deviceMemory, memoryOffset) );

		VK_CHECK( vkMapMemory(m_pContext->getLogicalDevice(), m_deviceMemory, memoryOffset, m_size, memoryMapFlags, &m_data) );
	}

	void BufferVulkan::close() {
		vkFreeMemory(m_pContext->getLogicalDevice(), m_deviceMemory, nullptr);

		vkDestroyBuffer(m_pContext->getLogicalDevice(), m_buffer, nullptr);
	}

	// can contain also optionalFlags, with which app may still run! Now we say, that there is only one way to do that
	uint32_t BufferVulkan::getMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties& memoryProperties, uint32_t memoryTypeBits, VkMemoryPropertyFlags flags) const {
		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
			if ((memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & flags) == flags) {
				return i;
			}
		}

		std::cout << "ERROR: no compatible memory type found!\n";
		return ~0u;
	}


}


#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {

	static VkImageMemoryBarrier beginRenderBarrier(VkImage image);
	static VkImageMemoryBarrier endRenderBarrier(VkImage image);


	void ContextVulkan::createCommandPool() {
		VkCommandPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
		createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
		createInfo.queueFamilyIndex = m_familyIndex;

		VK_CHECK( vkCreateCommandPool(m_device, &createInfo, nullptr, &m_commandPool) );
	}

	void ContextVulkan::closeCommandPool() const {
		vkDestroyCommandPool(m_device, m_commandPool, nullptr);
	}

	void ContextVulkan::createCommandBuffer() {
		VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocateInfo.commandPool = m_commandPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = 1;

		VK_CHECK( vkAllocateCommandBuffers(m_device, &allocateInfo, &m_commandBuffer) );
	}

	void ContextVulkan::beginCommandBuffer() {
		VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		VK_CHECK( vkBeginCommandBuffer(m_commandBuffer, &beginInfo) );

		const auto beginBarrier{ beginRenderBarrier(m_swapchain.images[m_imageIndex]) };
		vkCmdPipelineBarrier(m_commandBuffer,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_DEPENDENCY_BY_REGION_BIT,
			0,
			nullptr,
			0,
			nullptr,
			1,
			&beginBarrier
		);
	}

	void ContextVulkan::endCommandBuffer() {
		const auto endBarrier{ endRenderBarrier(m_swapchain.images[m_imageIndex]) };

		vkCmdPipelineBarrier(m_commandBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_DEPENDENCY_BY_REGION_BIT,
			0,
			nullptr,
			0,
			nullptr,
			1,
			&endBarrier
		);

		VK_CHECK(vkEndCommandBuffer(m_commandBuffer));
	}



	VkImageMemoryBarrier beginRenderBarrier(VkImage image) {
		VkImageMemoryBarrier result{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		result.srcAccessMask = VK_NULL_HANDLE;
		result.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		result.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		result.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		result.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		result.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		result.image = image;
		result.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		result.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;       // those flags may not be supported on android!
		result.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

		return result;
	}

	VkImageMemoryBarrier endRenderBarrier(VkImage image) {
		VkImageMemoryBarrier result{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		result.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		result.dstAccessMask = VK_NULL_HANDLE;
		result.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		result.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		result.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		result.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		result.image = image;
		result.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		result.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;       // those flags may not be supported on android!
		result.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

		return result;
	}


}



#ifndef MAR_ENGINE_PLATFORM_VULKAN_GRAPHICS_PIPELINE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_GRAPHICS_PIPELINE_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

	class ShadersVulkan;
	class ContextVulkan;


	class GraphicsPipelineVulkan {
	public:

		void create(ContextVulkan* pContext, const ShadersVulkan* shaderCollection);
		void close();

	private:

		void createDescriptorSetLayout();
		void createPipelineLayout();
		void createGraphicsPipeline(const ShadersVulkan* shaderCollection);

	public:

		const VkPipelineCache m_pipelineCache{ VK_NULL_HANDLE }; // critical for perfomance
		VkDescriptorSetLayout m_descriptorSetLayout{ VK_NULL_HANDLE };
		VkPipelineLayout m_pipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_pipeline{ VK_NULL_HANDLE };
		ContextVulkan* m_pContext{ nullptr };

	};

}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_GRAPHICS_PIPELINE_VULKAN_H

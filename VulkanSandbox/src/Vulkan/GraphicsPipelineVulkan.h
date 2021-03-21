

#ifndef MAR_ENGINE_PLATFORM_VULKAN_GRAPHICS_PIPELINE_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_GRAPHICS_PIPELINE_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

	class ShadersVulkan;
	class ContextVulkan;
	class BufferVulkan;


	class GraphicsPipelineVulkan {
	public:

		void create(ContextVulkan* pContext, const ShadersVulkan* pShaders, const std::vector<BufferVulkan>& ubos);
		void bind();
		void close();

		const VkPipelineLayout& getPipelineLayout() const;
		const std::vector<VkDescriptorSet>& getDescriptorSets() const;

	private:

		void createDescriptorLayout();
		void createPipelineLayout();
		void createGraphicsPipeline(const ShadersVulkan* pShaders);
		void createDescriptorPool();
		void createDescriptorSets(const std::vector<BufferVulkan>& ubos);


		const VkPipelineCache m_pipelineCache{ VK_NULL_HANDLE }; // critical for perfomance
		VkPipelineLayout m_pipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_pipeline{ VK_NULL_HANDLE };

		VkDescriptorSetLayout m_descriptorSetLayout;
		VkDescriptorPool m_descriptorPool;
		std::vector<VkDescriptorSet> m_descriptorSets;

		ContextVulkan* m_pContext{ nullptr };

	};

}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_GRAPHICS_PIPELINE_VULKAN_H

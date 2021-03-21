

#ifndef MAR_ENGINE_PLATFORM_VULKAN_SHADER_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_SHADER_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {

	class ContextVulkan;


	class ShadersVulkan {

		friend class GraphicsPipelineVulkan;

	public:

		void loadVertex(const char* path);
		void loadFragment(const char* path);

		void create(ContextVulkan* pContext);
		void close();

	private:

		void load(VkShaderModule& shaderModule, const char* path) const;

		void createDescriptorSetLayout();
		void closeDescriptorSetLayout();


		struct ShaderPaths {
			const char* vertex{ nullptr };
			const char* fragment{ nullptr };
		} m_paths;

		struct ShaderModules {
			VkShaderModule vertex;
			VkShaderModule fragment;
		} m_modules;

		VkDescriptorSetLayout m_descriptorSetLayout{ VK_NULL_HANDLE };

		ContextVulkan* m_pContext;

	};

}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_SHADER_VULKAN_H

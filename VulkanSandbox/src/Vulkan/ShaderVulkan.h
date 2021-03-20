

#ifndef MAR_ENGINE_PLATFORM_VULKAN_SHADER_VULKAN_H
#define MAR_ENGINE_PLATFORM_VULKAN_SHADER_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class ShaderVulkan {

		friend class GraphicsPipelineVulkan;

	public:

		void load(const char* path);
		void close() const;

	private:

		VkShaderModule m_shaderModule{ VK_NULL_HANDLE };
		std::string m_path;

	};

	class ShaderCollectionVulkan {

	public:

		ShaderVulkan& getVertex();
		ShaderVulkan& getFragment();

		void close();

	private:

		ShaderVulkan m_vertexShader;
		ShaderVulkan m_fragmentShader;
	
	};

}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_SHADER_VULKAN_H
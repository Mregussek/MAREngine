

#include "ShaderVulkan.h"
#include "LogicalDevVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


	void ShaderVulkan::load(const char* path) {
        FILE* file = fopen(path, "rb");
        fseek(file, 0, SEEK_END);
        const long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        std::vector<char> buffer(length);
        const size_t rc = fread(buffer.data(), 1, length, file);

        fclose(file);

        VkShaderModuleCreateInfo createInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
        createInfo.codeSize = length;
        createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
        
        VK_CHECK( vkCreateShaderModule(LogicalDevVulkan::Instance()->getDev(), &createInfo, nullptr, &m_shaderModule));
	    
        m_path = std::string(path);
    }
	
	void ShaderVulkan::close() const {
        vkDestroyShaderModule(LogicalDevVulkan::Instance()->getDev(), m_shaderModule, nullptr);
	}

    ShaderVulkan& ShaderCollectionVulkan::getVertex() {
        return m_vertexShader;
    }

    ShaderVulkan& ShaderCollectionVulkan::getFragment() {
        return m_fragmentShader;
    }

    void ShaderCollectionVulkan::close() {
        m_vertexShader.close();
        m_fragmentShader.close();
    }


}

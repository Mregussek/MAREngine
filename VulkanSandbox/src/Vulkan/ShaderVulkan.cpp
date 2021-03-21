

#include "ShaderVulkan.h"
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


    void ShadersVulkan::loadVertex(const char* path) {
        m_paths.vertex = path;
    }

    void ShadersVulkan::loadFragment(const char* path) {
        m_paths.fragment = path;
    }

    void ShadersVulkan::create(ContextVulkan* pContext) {
        m_pContext = pContext;

        // TODO: better shader paths validation
        if (m_paths.vertex != nullptr) {
            load(m_modules.vertex, m_paths.vertex);
        }
        if (m_paths.fragment != nullptr) {
            load(m_modules.fragment, m_paths.fragment);
        }
    }

    void ShadersVulkan::close() {
        if (m_paths.vertex != nullptr) {
            vkDestroyShaderModule(m_pContext->getLogicalDevice(), m_modules.vertex, nullptr);
        }
        if (m_paths.fragment != nullptr) {
            vkDestroyShaderModule(m_pContext->getLogicalDevice(), m_modules.fragment, nullptr);
        }
    }

    void ShadersVulkan::load(VkShaderModule& shaderModule, const char* path) const {
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

        VK_CHECK( vkCreateShaderModule(m_pContext->getLogicalDevice(), &createInfo, nullptr, &shaderModule) );
    }



}

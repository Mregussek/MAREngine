

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

        createDescriptorSetLayout();
    }

    void ShadersVulkan::close() {
        if (m_paths.vertex != nullptr) {
            vkDestroyShaderModule(m_pContext->getLogicalDevice(), m_modules.vertex, nullptr);
        }
        if (m_paths.fragment != nullptr) {
            vkDestroyShaderModule(m_pContext->getLogicalDevice(), m_modules.fragment, nullptr);
        }

        closeDescriptorSetLayout();
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

    void ShadersVulkan::createDescriptorSetLayout() {
        std::array<VkDescriptorSetLayoutBinding, 1> setBindings;
        setBindings[0].binding = 0;
        setBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        setBindings[0].descriptorCount = 1;
        setBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
        setLayoutCreateInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;
        setLayoutCreateInfo.bindingCount = setBindings.size();
        setLayoutCreateInfo.pBindings = setBindings.data();

        VK_CHECK( vkCreateDescriptorSetLayout(m_pContext->getLogicalDevice(), &setLayoutCreateInfo, nullptr, &m_descriptorSetLayout) );
    }

    void ShadersVulkan::closeDescriptorSetLayout() {
        vkDestroyDescriptorSetLayout(m_pContext->getLogicalDevice(), m_descriptorSetLayout, nullptr);
    }



}

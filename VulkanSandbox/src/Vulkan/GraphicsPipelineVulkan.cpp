

#include "GraphicsPipelineVulkan.h"
#include "../../VulkanLogging.h"
#include "LogicalDevVulkan.h"
#include "ShaderVulkan.h"


namespace mar {


    void GraphicsPipelineVulkan::create(ShaderCollectionVulkan& shaderCollection, VkRenderPass renderPass, VkViewport viewport, VkRect2D scissor) {
        createDescriptorSetLayout();
        createPipelineLayout();
        createGraphicsPipeline(shaderCollection, renderPass, viewport, scissor);
    }

    void GraphicsPipelineVulkan::close() {
        const auto& device = LogicalDevVulkan::Instance()->getDev();

        vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);

        vkDestroyPipeline(device, m_pipeline, nullptr);

        vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);

        vkDestroyPipelineCache(device, m_pipelineCache, nullptr);
    }

    void GraphicsPipelineVulkan::createDescriptorSetLayout() {
        std::array<VkDescriptorSetLayoutBinding, 1> setBindings;
        setBindings[0].binding = 0;
        setBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        setBindings[0].descriptorCount = 1;
        setBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
        setLayoutCreateInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;
        setLayoutCreateInfo.bindingCount = setBindings.size();
        setLayoutCreateInfo.pBindings = setBindings.data();

        VK_CHECK( vkCreateDescriptorSetLayout(LogicalDevVulkan::Instance()->getDev(), &setLayoutCreateInfo, nullptr, &m_descriptorSetLayout) );
    }

    void GraphicsPipelineVulkan::createPipelineLayout() {
        VkPipelineLayoutCreateInfo createInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
        createInfo.setLayoutCount = 1;
        createInfo.pSetLayouts = &m_descriptorSetLayout;

        VK_CHECK( vkCreatePipelineLayout(LogicalDevVulkan::Instance()->getDev(), &createInfo, nullptr, &m_pipelineLayout) );
    }

    void GraphicsPipelineVulkan::createGraphicsPipeline(ShaderCollectionVulkan& shaderCollection, VkRenderPass renderPass, VkViewport viewport, VkRect2D scissor) {
        VkPipelineVertexInputStateCreateInfo vertexInput{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizationState{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
        rasterizationState.lineWidth = 1.f;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizationState.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampleState{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
        multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleState.sampleShadingEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencilState{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

        VkPipelineColorBlendAttachmentState colorAttachmentState{};
        colorAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo colorBlendState{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
        colorBlendState.attachmentCount = 1;
        colorBlendState.pAttachments = &colorAttachmentState;

        constexpr std::array<VkDynamicState, 2> dynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

        VkPipelineDynamicStateCreateInfo dynamicState{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
        dynamicState.dynamicStateCount = dynamicStates.size();
        dynamicState.pDynamicStates = dynamicStates.data();

        std::array<VkPipelineShaderStageCreateInfo, 2> stages{
            VkPipelineShaderStageCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO },
            VkPipelineShaderStageCreateInfo{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO }
        };
        stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        stages[0].module = shaderCollection.getVertex().m_shaderModule;
        stages[0].pName = "main";
        stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        stages[1].module = shaderCollection.getFragment().m_shaderModule;
        stages[1].pName = "main";

        VkGraphicsPipelineCreateInfo createInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
        createInfo.stageCount = stages.size();
        createInfo.pStages = stages.data();
        createInfo.pVertexInputState = &vertexInput;
        createInfo.pInputAssemblyState = &inputAssembly;
        createInfo.pViewportState = &viewportState;
        createInfo.pRasterizationState = &rasterizationState;
        createInfo.pMultisampleState = &multisampleState;
        createInfo.pDepthStencilState = &depthStencilState;
        createInfo.pColorBlendState = &colorBlendState;
        createInfo.pDynamicState = &dynamicState;
        createInfo.layout = m_pipelineLayout;
        createInfo.renderPass = renderPass;

        VK_CHECK( vkCreateGraphicsPipelines(LogicalDevVulkan::Instance()->getDev(), m_pipelineCache, 1, &createInfo, nullptr, &m_pipeline) );
    }


}
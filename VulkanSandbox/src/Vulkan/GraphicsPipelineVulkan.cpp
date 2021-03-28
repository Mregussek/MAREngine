

#include "GraphicsPipelineVulkan.h"
#include "ContextVulkan.h"
#include "BufferVulkan.h"
#include "../../VulkanLogging.h"
#include "ShaderVulkan.h"


namespace mar {


    void GraphicsPipelineVulkan::create(ContextVulkan* pContext, ShadersVulkan* pShaders, const std::vector<BufferVulkan>& ubos) {
        m_pContext = pContext;

        createDescriptorLayout();
        createPipelineLayout();
        createGraphicsPipeline(pShaders);
        createDescriptorPool();
        createDescriptorSets(ubos);
    }

    void GraphicsPipelineVulkan::bind() {
        vkCmdBindPipeline(m_pContext->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    }

    void GraphicsPipelineVulkan::close() {
        const VkDevice& device{ m_pContext->getLogicalDevice() };

        vkDestroyDescriptorPool(device, m_descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
        vkDestroyPipeline(device, m_pipeline, nullptr);
        vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
        vkDestroyPipelineCache(device, m_pipelineCache, nullptr);
    }

    void GraphicsPipelineVulkan::createDescriptorLayout() {
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.pImmutableSamplers = nullptr;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        VK_CHECK(vkCreateDescriptorSetLayout(m_pContext->getLogicalDevice(), &layoutInfo, nullptr, &m_descriptorSetLayout));
    }

    void GraphicsPipelineVulkan::createPipelineLayout() {
        VkPipelineLayoutCreateInfo createInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
        createInfo.setLayoutCount = 1;
        createInfo.pSetLayouts = &m_descriptorSetLayout;

        VK_CHECK( vkCreatePipelineLayout(m_pContext->getLogicalDevice(), &createInfo, nullptr, &m_pipelineLayout) );
    }

    void GraphicsPipelineVulkan::createGraphicsPipeline(ShadersVulkan* pShaders) {
        VkVertexInputBindingDescription vertexBindingDescription{};
        vertexBindingDescription.binding = 0;
        vertexBindingDescription.stride = sizeof(objl::Vertex);
        vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        std::array<VkVertexInputAttributeDescription, 3> vertexInputsDescriptions{};
        vertexInputsDescriptions[0].binding = 0;
        vertexInputsDescriptions[0].location = 0;
        vertexInputsDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputsDescriptions[0].offset = offsetof(objl::Vertex, objl::Vertex::Position);

        vertexInputsDescriptions[1].binding = 0;
        vertexInputsDescriptions[1].location = 1;
        vertexInputsDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
        vertexInputsDescriptions[1].offset = offsetof(objl::Vertex, objl::Vertex::Normal);

        vertexInputsDescriptions[2].binding = 0;
        vertexInputsDescriptions[2].location = 2;
        vertexInputsDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        vertexInputsDescriptions[2].offset = offsetof(objl::Vertex, objl::Vertex::TextureCoordinate);

        VkPipelineVertexInputStateCreateInfo vertexInput{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
        vertexInput.vertexBindingDescriptionCount = 1;
        vertexInput.pVertexBindingDescriptions = &vertexBindingDescription;
        vertexInput.vertexAttributeDescriptionCount = vertexInputsDescriptions.size();
        vertexInput.pVertexAttributeDescriptions = vertexInputsDescriptions.data();

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkPipelineViewportStateCreateInfo viewportState{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
        viewportState.viewportCount = 1;
        viewportState.pViewports = &m_pContext->getViewport();
        viewportState.scissorCount = 1;
        viewportState.pScissors = &m_pContext->getScissor();

        VkPipelineRasterizationStateCreateInfo rasterizationState{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
        rasterizationState.lineWidth = 1.f;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizationState.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisampleState{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
        multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleState.sampleShadingEnable = VK_FALSE;

        VkPipelineDepthStencilStateCreateInfo depthStencilState{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };

        VkPipelineColorBlendAttachmentState colorAttachmentState{};
        colorAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorAttachmentState.blendEnable = VK_FALSE;

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
        stages[0].module = pShaders->m_modules.vertex;
        stages[0].pName = "main";
        stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        stages[1].module = pShaders->m_modules.fragment;
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
        createInfo.renderPass = m_pContext->getRenderPass();

        VK_CHECK( vkCreateGraphicsPipelines(m_pContext->getLogicalDevice(), m_pipelineCache, 1, &createInfo, nullptr, &m_pipeline) );

        pShaders->close();
    }

    void GraphicsPipelineVulkan::createDescriptorPool() {
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSize.descriptorCount = m_pContext->m_swapchain.images.size();

        VkDescriptorPoolCreateInfo poolInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = m_pContext->m_swapchain.images.size();

        VK_CHECK(vkCreateDescriptorPool(m_pContext->getLogicalDevice(), &poolInfo, nullptr, &m_descriptorPool));
    }

    void GraphicsPipelineVulkan::createDescriptorSets(const std::vector<BufferVulkan>& ubos) {
        std::vector<VkDescriptorSetLayout> layouts(m_pContext->m_swapchain.images.size(), m_descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
        allocInfo.descriptorPool = m_descriptorPool;
        allocInfo.descriptorSetCount = layouts.size();
        allocInfo.pSetLayouts = layouts.data();

        m_descriptorSets.resize(layouts.size());
        
        VK_CHECK(vkAllocateDescriptorSets(m_pContext->getLogicalDevice(), &allocInfo, m_descriptorSets.data()));

        for (size_t i = 0; i < layouts.size(); i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = ubos[i].m_buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = ubos[i].m_size;
        
            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = m_descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
        
            vkUpdateDescriptorSets(m_pContext->getLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
        }
    }

    const VkPipelineLayout& GraphicsPipelineVulkan::getPipelineLayout() const {
        return m_pipelineLayout;
    }

    const std::vector<VkDescriptorSet>& GraphicsPipelineVulkan::getDescriptorSets() const {
        return m_descriptorSets;
    }


}
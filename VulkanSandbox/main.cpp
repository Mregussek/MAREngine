
#include "VulkanInclude.h"
#include "VulkanLogging.h"
#include "src/Vulkan/ContextVulkan.h"
#include "src/Vulkan/PhysicalDevVulkan.h"
#include "src/Vulkan/LogicalDevVulkan.h"
#include "src/Vulkan/SwapchainVulkan.h"
#include "src/Vulkan/BufferVulkan.h"
#include "src/Vulkan/WindowSurfaceVulkan.h"
#include "src/Vulkan/DeviceQueueVulkan.h"
#include "src/Window/Window.h"
#include "src/Mesh/Mesh.h"


VkCommandPool createCommandPool(VkDevice device, uint32_t familyIndex) {
    VkCommandPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    createInfo.queueFamilyIndex = familyIndex;

    VkCommandPool commandPool{ 0 };
    VK_CHECK( vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) );

    return commandPool;
}

VkRenderPass createRenderPass(VkDevice device, VkFormat format) {
    std::array<VkAttachmentDescription, 1> attachments;
    attachments[0].format = format;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[0].flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;

    VkAttachmentReference colorAttachment{ 0 , VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachment;

    VkRenderPassCreateInfo createInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
    createInfo.attachmentCount = attachments.size();
    createInfo.pAttachments = attachments.data();
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;

    VkRenderPass renderPass{ 0 };
    VK_CHECK( vkCreateRenderPass(device, &createInfo, nullptr, &renderPass) );
    
    return renderPass;
}

VkCommandBuffer createCommandBuffer(VkDevice device, VkCommandPool commandPool) {
    VkCommandBufferAllocateInfo allocateInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocateInfo.commandPool = commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer{ 0 };

    VK_CHECK(vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer));

    return commandBuffer;
}

VkShaderModule loadShader(VkDevice device, const char* path) {
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

    VkShaderModule shaderModule{ 0 };
    VK_CHECK( vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) );

    return shaderModule;
}

VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device) {
    std::array<VkDescriptorSetLayoutBinding, 1> setBindings;
    setBindings[0].binding = 0;
    setBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    setBindings[0].descriptorCount = 1;
    setBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo setLayoutCreateInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO };
    setLayoutCreateInfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR;
    setLayoutCreateInfo.bindingCount = setBindings.size();
    setLayoutCreateInfo.pBindings = setBindings.data();

    VkDescriptorSetLayout setLayouts{ VK_NULL_HANDLE };
    VK_CHECK( vkCreateDescriptorSetLayout(device, &setLayoutCreateInfo, nullptr, &setLayouts) );

    return setLayouts;
}

VkPipelineLayout createPipelineLayout(VkDevice device, VkDescriptorSetLayout setLayout) {
    VkPipelineLayoutCreateInfo createInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
    createInfo.setLayoutCount = 1;
    createInfo.pSetLayouts = &setLayout;

    VkPipelineLayout layout{ VK_NULL_HANDLE };
    VK_CHECK(vkCreatePipelineLayout(device, &createInfo, nullptr, &layout));

    return layout;
}

VkPipeline createGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, VkPipelineCache pipelineCache, 
    VkViewport viewport, VkRect2D scissor, VkShaderModule vertex, VkShaderModule fragment) 
{
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
    stages[0].module = vertex;
    stages[0].pName = "main";
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = fragment;
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
    createInfo.layout = layout;
    createInfo.renderPass = renderPass;

    VkPipeline pipeline{ 0 };
    VK_CHECK(vkCreateGraphicsPipelines(device, pipelineCache, 1, &createInfo, nullptr, &pipeline));

    return pipeline;
}

VkImageMemoryBarrier pipelineBarrierImage(VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldLayout, VkImageLayout newLayout)  {
    VkImageMemoryBarrier result{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    result.srcAccessMask = srcAccessMask;
    result.dstAccessMask = dstAccessMask;
    result.oldLayout = oldLayout;
    result.newLayout = newLayout;
    result.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    result.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    result.image = image;
    result.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    result.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;       // those flags may not be supported on android!
    result.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;

    return result;
}

int main(void) {
    mar::Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    mar::ContextVulkan contextVk{};
    contextVk.create();
    
    mar::PhysicalDevVulkan physicalDevVk{};
    physicalDevVk.create();

    mar::LogicalDevVulkan deviceVk{};
    deviceVk.create();

    mar::WindowSurfaceVulkan windowSurface{};
    windowSurface.create();

    VkExtent2D windowSize{ (uint32_t)window.getWidth() , (uint32_t)window.getHeight() };

    const VkViewport viewport{ 0.f , (float)window.getHeight(), (float)window.getWidth(), -(float)window.getHeight(), 0.f, 1.f };
    const VkRect2D scissor{ {0, 0 }, windowSize };

    constexpr VkPipelineCache pipelineCache{ VK_NULL_HANDLE }; // critical for perfomance

    const auto renderPass{ createRenderPass(deviceVk.getDev(), windowSurface.getSwapchainFormat()) };
    const auto commandPool{ createCommandPool(deviceVk.getDev(), physicalDevVk.getFamilyIndex()) };
    const auto commandBuffer{ createCommandBuffer(deviceVk.getDev(), commandPool) };
    
    mar::DeviceQueueVulkan deviceQueueVk{};
    deviceQueueVk.create();

    const auto triangleSetLayout{ createDescriptorSetLayout(deviceVk.getDev()) };
    const auto triangleLayout{ createPipelineLayout(deviceVk.getDev(), triangleSetLayout) };

    const auto vertexShader{ loadShader(deviceVk.getDev(), "resources/triangle.vert.spv") };
    const auto fragmentShader{ loadShader(deviceVk.getDev(), "resources/triangle.frag.spv") };

    const auto trianglePipeline{ createGraphicsPipeline(deviceVk.getDev(), renderPass, triangleLayout, pipelineCache, viewport, scissor, vertexShader, fragmentShader) };

    mar::SwapchainVulkan swapchainStruct(windowSize);
    swapchainStruct.create(windowSurface, renderPass);

    mar::Mesh mesh{};
    mesh.loadFromFile("resources/monkey.obj");

    mar::BufferVulkan vertexBuffer{};
    vertexBuffer.create(deviceVk.getDev(), 128 * 1024 * 1024, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    vertexBuffer.update(mesh.m_vertices);

    mar::BufferVulkan indexBuffer{};
    indexBuffer.create(deviceVk.getDev(), 128 * 1024 * 1024, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    indexBuffer.update(mesh.m_indices);

    while (window.shouldClose()) {
        VkSurfaceCapabilitiesKHR surfaceCaps;
        VK_CHECK( vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevVk.getPhyDev(), windowSurface.getSurface(), &surfaceCaps) );
        const bool windowSizeHasChanged = !(swapchainStruct.extent.width == surfaceCaps.currentExtent.width && swapchainStruct.extent.height == surfaceCaps.currentExtent.height);
        if (windowSizeHasChanged) {
            swapchainStruct.resizeIfNecessary(windowSurface, surfaceCaps, renderPass);
            windowSize.height = window.getHeight();
            windowSize.width = window.getWidth();
        }

        uint32_t imageIndex{ 0 };
        deviceQueueVk.prepare(swapchainStruct.swapchain, imageIndex);

        VK_CHECK( vkResetCommandPool(deviceVk.getDev(), commandPool, 0) );

        VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK( vkBeginCommandBuffer(commandBuffer, &beginInfo) );
        
        const auto renderBeginBarrier{ pipelineBarrierImage(swapchainStruct.images[imageIndex], 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &renderBeginBarrier);

        constexpr VkClearColorValue clearColor{ 48.f / 255.f, 10.f / 255.f, 36.f / 255.f ,1 };
        constexpr std::array<VkClearValue, 1> clearValue{ clearColor };

        VkRenderPassBeginInfo passBeginInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        passBeginInfo.renderPass = renderPass;
        passBeginInfo.framebuffer = swapchainStruct.framebuffers[imageIndex];
        passBeginInfo.renderArea.extent = swapchainStruct.extent;
        passBeginInfo.clearValueCount = clearValue.size();
        passBeginInfo.pClearValues = clearValue.data();

        vkCmdBeginRenderPass(commandBuffer, &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE); // draw calls are below

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, trianglePipeline);

        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = vertexBuffer.m_buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = vertexBuffer.m_size;

        std::array<VkWriteDescriptorSet, 1> descriptors{ VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
        descriptors[0].dstBinding = 0;
        descriptors[0].descriptorCount = 1;
        descriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptors[0].pBufferInfo = &bufferInfo;
        descriptors[0].pNext = nullptr;

        vkCmdPushDescriptorSetKHR(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, triangleLayout, 0, descriptors.size(), descriptors.data());

        vkCmdBindIndexBuffer(commandBuffer, indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, (uint32_t)mesh.m_indices.size(), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        const auto renderEndBarrier{ pipelineBarrierImage(swapchainStruct.images[imageIndex], VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &renderEndBarrier);

        VK_CHECK( vkEndCommandBuffer(commandBuffer) );

        deviceQueueVk.draw(commandBuffer, swapchainStruct.swapchain, imageIndex);
        
        deviceVk.endPendingJobs();

        window.pollEvents();
    }

    deviceVk.endPendingJobs();

    vertexBuffer.close(deviceVk.getDev());
    indexBuffer.close(deviceVk.getDev());

    swapchainStruct.close();

    deviceQueueVk.close();

    vkDestroyDescriptorSetLayout(deviceVk.getDev(), triangleSetLayout, nullptr);

    vkDestroyPipeline(deviceVk.getDev(), trianglePipeline, nullptr);

    vkDestroyPipelineLayout(deviceVk.getDev(), triangleLayout, nullptr);

    vkDestroyPipelineCache(deviceVk.getDev(), pipelineCache, nullptr);

    vkDestroyShaderModule(deviceVk.getDev(), vertexShader, nullptr);
    vkDestroyShaderModule(deviceVk.getDev(), fragmentShader, nullptr);

    vkDestroyCommandPool(deviceVk.getDev(), commandPool, nullptr);

    vkDestroyRenderPass(deviceVk.getDev(), renderPass, nullptr);

    windowSurface.close();

    deviceVk.close();

    contextVk.close();

    window.terminate();

    exit(EXIT_SUCCESS);
}
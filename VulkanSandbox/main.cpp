
#include "VulkanInclude.h"
#include "VulkanLogging.h"
#include "src/Vulkan/ContextVulkan.h"
#include "src/Vulkan/BufferVulkan.h"
#include "src/Vulkan/ShaderVulkan.h"
#include "src/Vulkan/GraphicsPipelineVulkan.h"
#include "src/Window/Window.h"
#include "src/Mesh/Mesh.h"



int main(void) {
    mar::Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    mar::ContextVulkan contextVk{};
    contextVk.initialize(&window);

    mar::ShadersVulkan shaders{};
    shaders.loadVertex("resources/triangle.vert.spv");
    shaders.loadFragment("resources/triangle.frag.spv");
    shaders.create(&contextVk);

    mar::GraphicsPipelineVulkan graphicsPipelineVk{};
    graphicsPipelineVk.create(&contextVk, &shaders);

    mar::Mesh mesh{};
    mesh.loadFromFile("resources/monkey.obj");

    mar::BufferVulkan vertexBuffer{};
    vertexBuffer.create(&contextVk, 128 * 1024 * 1024, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    vertexBuffer.update(mesh.m_vertices);

    mar::BufferVulkan indexBuffer{};
    indexBuffer.create(&contextVk, 128 * 1024 * 1024, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    indexBuffer.update(mesh.m_indices);

    while (window.shouldClose()) {
        contextVk.beginFrame();

        vkCmdBindPipeline(contextVk.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineVk.m_pipeline);

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

        vkCmdPushDescriptorSetKHR(contextVk.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineVk.m_pipelineLayout, 0, descriptors.size(), descriptors.data());

        vkCmdBindIndexBuffer(contextVk.getCommandBuffer(), indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(contextVk.getCommandBuffer(), (uint32_t)mesh.m_indices.size(), 1, 0, 0, 0);

        contextVk.endFrame();
        contextVk.endPendingJobs();

        window.pollEvents();
    }

    contextVk.endPendingJobs();

    vertexBuffer.close();
    indexBuffer.close();

    graphicsPipelineVk.close();

    shaders.close();

    contextVk.terminate();

    window.terminate();

    return 0;
}

#include "VulkanInclude.h"
#include "VulkanLogging.h"
#include "src/Vulkan/ContextVulkan.h"
#include "src/Vulkan/BufferVulkan.h"
#include "src/Vulkan/ShaderVulkan.h"
#include "src/Vulkan/GraphicsPipelineVulkan.h"
#include "src/Window/Window.h"
#include "src/Mesh/Mesh.h"


using namespace marengine;


int main(void) {
    mar::Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    mar::ContextVulkan contextVk{};
    contextVk.initialize(&window);

    mar::ShadersVulkan shaders{};
    shaders.loadVertex("resources/triangle.vert.spv");
    shaders.loadFragment("resources/triangle.frag.spv");
    shaders.create(&contextVk);

    mar::Mesh mesh{};
    mesh.loadFromFile("resources/monkey.obj");

    mar::BufferVulkan vertexBuffer{};
    vertexBuffer.create(&contextVk, 128 * 1024 * 1024, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    vertexBuffer.update(mesh.m_vertices);

    mar::BufferVulkan indexBuffer{};
    indexBuffer.create(&contextVk, 128 * 1024 * 1024, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    indexBuffer.update(mesh.m_indices);

    auto trans = maths::mat4::translation({0.f, 0.f, 2.f});
    auto view = maths::mat4::lookAt({ 0.f, 0.0f, 0.0f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 0.f });
    auto proj = maths::mat4::perspective(45.f, 1.333f, 0.01f, 100.f);
    auto mvp = proj * view * trans;

    std::vector<mar::BufferVulkan> cameraUBOs(contextVk.getImagesCount());
    for (size_t i = 0; i < contextVk.getImagesCount(); i++) {
        cameraUBOs[i].create(&contextVk, sizeof(mvp), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        cameraUBOs[i].update(maths::mat4::value_ptr(mvp), sizeof(mvp));
    }

    mar::GraphicsPipelineVulkan graphicsPipelineVk{};
    graphicsPipelineVk.create(&contextVk, &shaders, cameraUBOs);

    while (window.shouldClose()) {
        contextVk.beginFrame();

        graphicsPipelineVk.bind();
        
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(contextVk.getCommandBuffer(), 0, 1, &vertexBuffer.m_buffer, offsets);
        vkCmdBindIndexBuffer(contextVk.getCommandBuffer(), indexBuffer.m_buffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(contextVk.getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipelineVk.getPipelineLayout(), 0, 1, graphicsPipelineVk.getDescriptorSets().data(), 0, nullptr);

        vkCmdDrawIndexed(contextVk.getCommandBuffer(), (uint32_t)mesh.m_indices.size(), 1, 0, 0, 0);

        contextVk.endFrame();
        contextVk.endPendingJobs();

        window.pollEvents();
    }

    contextVk.endPendingJobs();

    vertexBuffer.close();
    indexBuffer.close();
    for (auto& ubo : cameraUBOs) {
        ubo.close();
    }

    graphicsPipelineVk.close();

    shaders.close();

    contextVk.terminate();

    window.terminate();

    return 0;
}
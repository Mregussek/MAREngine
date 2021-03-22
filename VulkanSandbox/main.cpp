
#include "VulkanInclude.h"
#include "VulkanLogging.h"
#include "src/Vulkan/ContextVulkan.h"
#include "src/Vulkan/BufferVulkan.h"
#include "src/Vulkan/ShaderVulkan.h"
#include "src/Vulkan/GraphicsPipelineVulkan.h"
#include "src/Window/Window.h"
#include "src/Mesh/Mesh.h"


using namespace marengine;


class Camera {
public:

    maths::mat4 handleCamera(mar::Window* pWindow);

private:

    void processInput(mar::Window* pWindow);
    bool processMousePosition(mar::Window* pWindow, bool firstMouse);

    maths::vec3 m_pos{ 0.f, 0.f, 0.f };
    maths::vec3 m_front{ 0.f, 0.f, 1.f };
    maths::vec3 m_right{ 1.f, 0.f, 0.f };
    maths::vec3 m_up{ 0.f, 1.f, 0.f };
    float m_pitch{ 0.f };
    float m_yaw{ 0.f };

};


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

    auto getEntityTransform = []()->maths::mat4 {
        return maths::mat4::translation({ 2.f, 0.f, 0.f })* maths::mat4::rotation((float)glfwGetTime(), { 1.f, 1.f, 0.f });
    };

    Camera camera;
    auto mvp = camera.handleCamera(&window) * getEntityTransform();
    
    std::vector<mar::BufferVulkan> cameraUBOs(contextVk.getImagesCount());
    for (size_t i = 0; i < contextVk.getImagesCount(); i++) {
        cameraUBOs[i].create(&contextVk, sizeof(mvp), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        cameraUBOs[i].update(maths::mat4::value_ptr(mvp), sizeof(mvp));
    }

    mar::GraphicsPipelineVulkan graphicsPipelineVk{};
    graphicsPipelineVk.create(&contextVk, &shaders, cameraUBOs);

    while (window.shouldClose()) {
        mvp = camera.handleCamera(&window) * getEntityTransform();
        for (size_t i = 0; i < contextVk.getImagesCount(); i++) {
            cameraUBOs[i].update(maths::mat4::value_ptr(mvp), sizeof(mvp));
        }
        
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


maths::mat4 Camera::handleCamera(mar::Window* pWindow) {
    using namespace maths;

    static bool firstMouse{ true };
    static bool holdingRightMouseButton{ false };
    const bool userPressedRightMouseButton{ pWindow->isMousePressed(GLFW_MOUSE_BUTTON_2) };

    if (userPressedRightMouseButton) {
        if (!holdingRightMouseButton) { firstMouse = true; }
        processInput(pWindow);
        if (processMousePosition(pWindow, firstMouse)) {
            firstMouse = false;
        }
    }
    else {
        firstMouse = true;
    }

    holdingRightMouseButton = userPressedRightMouseButton;

    const maths::vec3 worldUp{ 0.f, 1.f, 0.f };
    const auto yawRad{ trig::toRadians(m_yaw) };
    const auto pitchRad{ trig::toRadians(m_pitch) };
    const maths::vec3 front{
        {trig::cosine(yawRad) * trig::cosine(pitchRad)},
        {trig::sine(pitchRad)},
        {trig::sine(yawRad) * trig::cosine(pitchRad)}
    };

    m_front = vec3::normalize(front);
    m_right = vec3::normalize(vec3::cross(m_front, worldUp));
    m_up = vec3::normalize(vec3::cross(m_right, m_front));

    auto view = mat4::lookAt(m_pos, m_pos + m_front, m_up);
    auto proj = mat4::perspective(45.f, 1.333f, 0.01f, 100.f);
    return proj * view;
}

void Camera::processInput(mar::Window* pWindow) {
    static float lastTime{ 0.0f };
    const float currentFrame{ (float)glfwGetTime() };
    const float deltaTime{ currentFrame - lastTime };
    lastTime = currentFrame;

    if (pWindow->isKeyPressed(GLFW_KEY_W)) {
        m_pos = m_pos + (m_front * deltaTime);
    }
    if (pWindow->isKeyPressed(GLFW_KEY_S)) {
        m_pos = m_pos - (m_front * deltaTime);
    }
    if (pWindow->isKeyPressed(GLFW_KEY_A)) {
        m_pos = m_pos - (m_right * deltaTime);
    }
    if (pWindow->isKeyPressed(GLFW_KEY_D)) {
        m_pos = m_pos + (m_right * deltaTime);
    }
}

bool Camera::processMousePosition(mar::Window* pWindow, bool firstMouse) {
    static float lastX{ 0.f };
    static float lastY{ 0.f };

    const auto [xoffset, yoffset] = pWindow->getMousePosition();

    if (lastX == xoffset && lastY == yoffset) { return false; }

    if (firstMouse) {
        lastX = xoffset;
        lastY = yoffset;
    }

    constexpr float mouseSensitivity{ 0.2f };

    const float x_position = (xoffset - lastX) * mouseSensitivity;
    const float y_position = (lastY - yoffset) * mouseSensitivity;

    lastX = xoffset;
    lastY = yoffset;

    m_yaw += x_position;
    m_pitch += y_position;

    constexpr bool constrainPaith{ true };
    if constexpr (constrainPaith) {
        if (m_pitch > 89.0f) { m_pitch = 89.0f; }
        if (m_pitch < -89.f) { m_pitch = -89.f; };
    }

    return true;
}
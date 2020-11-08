
#include "VulkanInclude.h"
#include "src/Window.h"


/*
There is no need to include all those files in MAREngine!

remote: warning: GH001: Large files detected. You may want to try Git Large File Storage - https://git-lfs.github.com.
remote: warning: See http://git.io/iEPt8g for more information.
remote: warning: File VulkanSandbox/3rd_party/Vulkan/Lib32/SPIRV-Tools-opt.lib is 81.30 MB; this is larger than GitHub's recommended maximum file size of 50.00 MB
remote: warning: File VulkanSandbox/3rd_party/Vulkan/Lib32/shaderc_combined.lib is 56.33 MB; this is larger than GitHub's recommended maximum file size of 50.00 MB
*/

#define VK_CHECK(call) \
    do { \
        const VkResult result = call; \
    } while(0)


VkInstance createVulkanInstance() {
    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO }; // check if 1.2 is available via vkEnumerateInstanceVersion
    appInfo.apiVersion = VK_API_VERSION_1_2;

    constexpr std::array<const char*, 1> debugLayers{
        "VK_LAYER_KHRONOS_validation"
    };

#if defined(VK_USE_PLATFORM_WIN32_KHR)
    constexpr std::array<const char*, 2> extensions = {
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#else
    constexpr std::array<const char*, 1> extensions = {
#endif
        VK_KHR_SURFACE_EXTENSION_NAME
    };

    VkInstanceCreateInfo createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    createInfo.pApplicationInfo = &appInfo;
    createInfo.ppEnabledLayerNames = debugLayers.data();
    createInfo.enabledLayerCount = debugLayers.size();
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledExtensionCount = extensions.size();

    VkInstance instance{ 0 };

    VK_CHECK( vkCreateInstance(&createInfo, nullptr, &instance) );

    return instance;
}

VkPhysicalDevice createPhysicalDevice(VkInstance instance) {
    std::array<VkPhysicalDevice, 16> physicalDevices;
    uint32_t physicalDeviceCount{ physicalDevices.size() };
    VK_CHECK( vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()) );

    return [&physicalDevices]() {
        const auto it = std::find_if(physicalDevices.cbegin(), physicalDevices.cend(), [](const VkPhysicalDevice physicalDevice) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physicalDevice, &properties);
            return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        });

        if (it != physicalDevices.end()) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(*it, &properties);
            std::cout << "Picking: " << properties.deviceName << "\n";
            return *it;
        }
        else if (physicalDevices.size() > 0) {
            VkPhysicalDeviceProperties fallbackProperties;
            vkGetPhysicalDeviceProperties(physicalDevices[0], &fallbackProperties);
            std::cout << "Picking fallback: " << fallbackProperties.deviceName << "\n";
            return physicalDevices[0];
        }

        std::cout << "No physical devices available!\n";
        return VkPhysicalDevice{ VK_NULL_HANDLE };
    }(); 
}

VkDevice createDevice(VkPhysicalDevice physicalDevice, uint32_t familyIndex) {
    constexpr std::array<const char*, 1> extensions{
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    float queueProperties{ 1.f };

    VkDeviceQueueCreateInfo queueInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueInfo.queueFamilyIndex = familyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queueProperties;

    VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDevice device{ 0 };
    VK_CHECK( vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) );

    return device;
}

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window) {
    if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
        VkWin32SurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
        createInfo.hinstance = GetModuleHandle(0);
        createInfo.hwnd = glfwGetWin32Window(window);
        VkSurfaceKHR surface{ 0 };
        VK_CHECK( vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) );
        return surface;
    }
    else { // other platforms
        return VkSurfaceKHR{ VK_NULL_HANDLE };
    }
}

VkFormat getSwapchainFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    std::array<VkSurfaceFormatKHR, 16> formats;
    uint32_t formatCount = formats.size();
    VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data()) );

    return formats[0].format;
}

VkSwapchainKHR createSwapchain(VkDevice device, VkSurfaceKHR surface, VkFormat format, int32_t width, int32_t height, const uint32_t* familyIndex) {

    VkSwapchainCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    createInfo.surface = surface;
    createInfo.minImageCount = 2;
    createInfo.imageFormat = format;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent.width = width;
    createInfo.imageExtent.height = height;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = familyIndex;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    VkSwapchainKHR swapchain{ 0 };

    VK_CHECK( vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) );

    return swapchain;
}

VkSemaphore createSemaphore(VkDevice device) {
    VkSemaphoreCreateInfo createInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

    VkSemaphore semaphore{ 0 };
    VK_CHECK(vkCreateSemaphore(device, &createInfo, nullptr, &semaphore));

    return semaphore;
}

VkCommandPool createCommandPool(VkDevice device, uint32_t familyIndex) {
    VkCommandPoolCreateInfo createInfo{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    createInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    createInfo.queueFamilyIndex = familyIndex;

    VkCommandPool commandPool{ 0 };
    VK_CHECK( vkCreateCommandPool(device, &createInfo, nullptr, &commandPool) );

    return commandPool;
}

VkRenderPass createRenderPass(VkDevice device, VkFormat format) {
    constexpr uint32_t index = 0;
    std::array<VkAttachmentDescription, 1> attachments;
    attachments[index].format = format;
    attachments[index].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[index].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[index].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[index].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[index].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[index].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachments[index].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachment{ index , VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

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

VkImageView createImageView(VkDevice device, VkImage image, VkFormat format) {
    VkImageViewCreateInfo createInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    createInfo.image = image;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = format;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.layerCount = 1;

    VkImageView view{ 0 };

    VK_CHECK( vkCreateImageView(device, &createInfo, nullptr, &view) );

    return view;
}

VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, uint32_t width, uint32_t height) {
    VkFramebufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
    createInfo.renderPass = renderPass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &imageView;
    createInfo.width = width;
    createInfo.height = height;
    createInfo.layers = 1;

    VkFramebuffer framebuffer{ 0 };

    VK_CHECK( vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffer) );

    return framebuffer;
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
    char* buffer = new char[length];
    size_t rc = fread(buffer, 1, length, file);
    fclose(file);

    VkShaderModuleCreateInfo createInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    createInfo.codeSize = length;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer);

    VkShaderModule shaderModule{ 0 };
    VK_CHECK( vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) );
    
    delete[] buffer;

    return shaderModule;
}

VkPipelineLayout createPipelineLayout(VkDevice device) {
    VkPipelineLayoutCreateInfo createInfo{ VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
    VkPipelineLayout layout{ 0 };

    VK_CHECK( vkCreatePipelineLayout(device, &createInfo, nullptr, &layout) );

    return layout;
}

VkPipeline createGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, VkPipelineCache pipelineCache, VkShaderModule vertex, VkShaderModule fragment) {
    VkGraphicsPipelineCreateInfo createInfo = { VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };

    std::array<VkPipelineShaderStageCreateInfo, 2> stages{};
    stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = vertex;
    stages[0].pName = "main";
    stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = fragment;
    stages[1].pName = "main";

    createInfo.stageCount = stages.size();
    createInfo.pStages = stages.data();

    VkPipelineVertexInputStateCreateInfo vertexInput = { VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
    createInfo.pVertexInputState = &vertexInput;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = { VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    createInfo.pInputAssemblyState = &inputAssembly;

    VkPipelineViewportStateCreateInfo viewportState = { VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    createInfo.pViewportState = &viewportState;

    VkPipelineRasterizationStateCreateInfo rasterizationState = { VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
    rasterizationState.lineWidth = 1.f;
    createInfo.pRasterizationState = &rasterizationState;

    VkPipelineMultisampleStateCreateInfo multisampleState = { VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.pMultisampleState = &multisampleState;

    VkPipelineDepthStencilStateCreateInfo depthStencilState = { VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
    createInfo.pDepthStencilState = &depthStencilState;

    VkPipelineColorBlendAttachmentState colorAttachmentState = {};
    colorAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendState = { VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &colorAttachmentState;
    createInfo.pColorBlendState = &colorBlendState;

    constexpr std::array<VkDynamicState, 2> dynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicState = { VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
    dynamicState.dynamicStateCount = dynamicStates.size();
    dynamicState.pDynamicStates = dynamicStates.data();
    createInfo.pDynamicState = &dynamicState;

    createInfo.layout = layout;
    createInfo.renderPass = renderPass;

    VkPipeline pipeline{ 0 };

    VK_CHECK( vkCreateGraphicsPipelines(device, pipelineCache, 1, &createInfo, nullptr, &pipeline) );

    return pipeline;
}

int main(void) {
    Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    uint32_t familyIndex{ 0 }; // this may produce validation error
    const auto instance = createVulkanInstance();
    const auto physicalDevice = createPhysicalDevice(instance);
    const auto device = createDevice(physicalDevice, familyIndex);
    const auto surface = createSurface(instance, window.getWindow());
    const auto swapchainFormat = getSwapchainFormat(physicalDevice, surface);
    const auto swapchain = createSwapchain(device, surface, swapchainFormat, window.getWidth(), window.getHeight(), &familyIndex);
    const auto releaseSemaphore = createSemaphore(device);
    const auto acquireSemaphore = createSemaphore(device);
    const auto renderPass = createRenderPass(device, swapchainFormat);
    const auto commandPool = createCommandPool(device, familyIndex);
    const auto commandBuffer = createCommandBuffer(device, commandPool);
    const auto triangleVertShader = loadShader(device, "resources/triangle.vert.spv");
    const auto triangleFragShader = loadShader(device, "resources/triangle.frag.spv");
    VkPipelineCache pipelineCache{ 0 }; // critical for perfomance
    const auto triangleLayout = createPipelineLayout(device);
    const auto trianglePipeline = createGraphicsPipeline(device, renderPass, triangleLayout, pipelineCache, triangleVertShader, triangleFragShader);

    VkQueue queue{ 0 };
    vkGetDeviceQueue(device, familyIndex, 0, &queue);

    constexpr size_t swapchainSize = 16;
    std::array<VkImage, swapchainSize> swapchainImages;    // it should be dynamically allocated?
    uint32_t swapchainImageCount = swapchainImages.size();
    VK_CHECK( vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data()) );

    std::array<VkImageView, swapchainSize> swapchainImageViews;
    for (size_t i = 0; i < swapchainImageCount; i++) {
        swapchainImageViews[i] = createImageView(device, swapchainImages[i], swapchainFormat);
    }

    std::array<VkFramebuffer, swapchainSize> swapchainFramebuffers;
    for (size_t i = 0; i < swapchainImageCount; i++) {
        swapchainFramebuffers[i] = createFramebuffer(device, renderPass, swapchainImageViews[i], window.getWidth(), window.getHeight());
    }

    while (window.shouldClose()) {

        uint32_t imageIndex{ 0 };
        VK_CHECK( vkAcquireNextImageKHR(device, swapchain, ~0ull, acquireSemaphore, VK_NULL_HANDLE, &imageIndex) );

        VK_CHECK( vkResetCommandPool(device, commandPool, 0) );

        VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK( vkBeginCommandBuffer(commandBuffer, &beginInfo) );

        constexpr VkClearColorValue clearColor{ 48.f / 255.f, 10.f / 255.f, 36.f / 255.f ,1 };
        constexpr VkClearValue clearValue{ clearColor };

        VkRenderPassBeginInfo passBeginInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        passBeginInfo.renderPass = renderPass;
        passBeginInfo.framebuffer = swapchainFramebuffers[imageIndex];
        passBeginInfo.renderArea.extent.width = window.getWidth();
        passBeginInfo.renderArea.extent.height = window.getHeight();
        passBeginInfo.clearValueCount = 1;
        passBeginInfo.pClearValues = &clearValue;

        vkCmdBeginRenderPass(commandBuffer, &passBeginInfo, VK_SUBPASS_CONTENTS_INLINE); // draw calls are below

        VkViewport viewport{ 0.f , 0.f, (float)window.getWidth(), (float)window.getHeight(), 0.f, 1.f };
        VkRect2D scissor{ {0, 0 }, { window.getWidth(), window.getHeight() } };

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, trianglePipeline);
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        VK_CHECK( vkEndCommandBuffer(commandBuffer) );

        constexpr VkPipelineStageFlags submitStageMask{ VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

        VkSubmitInfo submitInfo{ VK_STRUCTURE_TYPE_SUBMIT_INFO };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &acquireSemaphore;
        submitInfo.pWaitDstStageMask = &submitStageMask;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &releaseSemaphore;

        VK_CHECK( vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) );

        VkPresentInfoKHR presentInfo{ VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &releaseSemaphore;

        VK_CHECK( vkQueuePresentKHR(queue, &presentInfo) );

        VK_CHECK( vkDeviceWaitIdle(device) );

        window.pollEvents();
    }

    window.terminate();

    exit(EXIT_SUCCESS);
}
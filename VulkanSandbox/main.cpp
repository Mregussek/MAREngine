
#include "VulkanInclude.h"
#include "src/Window.h"


#define VK_CHECK(call) \
    do { \
        const VkResult result = call; \
    } while(0)



uint32_t getVulkanApiVersion() {
    uint32_t apiVersion{ 0 };
    VK_CHECK(vkEnumerateInstanceVersion(&apiVersion));

    std::cout << "Supported version: " << VK_VERSION_MAJOR(apiVersion) << "." << VK_VERSION_MINOR(apiVersion) <<
        "." << VK_VERSION_PATCH(apiVersion) << "\n";

    return apiVersion;
}

bool pushExtensionIfAvailable(std::vector<const char*>& extensionsToEnable, std::vector<VkExtensionProperties>& allExtensions, const char* extensionProperty) {
    const auto availableExtension = [&extensionProperty](const VkExtensionProperties extension) {
        return std::strcmp(extension.extensionName, extensionProperty) == 0;
    };

    if (std::find_if(allExtensions.cbegin(), allExtensions.cend(), availableExtension) != allExtensions.cend()) {
        extensionsToEnable.emplace_back(extensionProperty);
        return true;
    }

    return false;
}

void fillNeededInstanceExtensions(std::vector<const char*>& extensionToEnable) {
    uint32_t extensionCount{ 0 };
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr));

    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data()));

    const bool isAvailableKHRWin32Surface = pushExtensionIfAvailable(extensionToEnable, extensionProperties, VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
    const bool isAvailableDebugReport = pushExtensionIfAvailable(extensionToEnable, extensionProperties, VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    const bool isAvailableKHRSurface = pushExtensionIfAvailable(extensionToEnable, extensionProperties, VK_KHR_SURFACE_EXTENSION_NAME);
}

void fillNeededLayers(std::vector<const char*>& layersToEnable) {
    uint32_t layerCount{ 0 };
    VK_CHECK( vkEnumerateInstanceLayerProperties(&layerCount, nullptr) );

    std::vector<VkLayerProperties> availableLayers(layerCount);
    VK_CHECK( vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()) );

    const auto pushLayerIfAvailable = [&layersToEnable, &availableLayers](const char* layerProperty)->bool {
        const auto availableProperty = [&layerProperty](const VkLayerProperties property) {
            return std::strcmp(property.layerName, layerProperty) == 0;
        };

        if (std::find_if(availableLayers.begin(), availableLayers.end(), availableProperty) != availableLayers.end()) {
            layersToEnable.emplace_back(layerProperty);
            return true;
        }

        return false;
    };
    
    const bool isAvailableKHRONOSValidation = pushLayerIfAvailable("VK_LAYER_KHRONOS_validation");
}

VkInstance createVulkanInstance() {
    const auto apiVersion = getVulkanApiVersion();

    std::vector<const char*> layersToEnable;
    fillNeededLayers(layersToEnable);

    std::vector<const char*> extensionToEnable;
    fillNeededInstanceExtensions(extensionToEnable);

    VkApplicationInfo appInfo{ VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.apiVersion = apiVersion;
    appInfo.pApplicationName = "Hello, Vulkan!";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName = "MAREngine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);

    VkInstanceCreateInfo createInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    createInfo.pApplicationInfo = &appInfo;
    createInfo.ppEnabledLayerNames = layersToEnable.data();
    createInfo.enabledLayerCount = (uint32_t)layersToEnable.size();
    createInfo.ppEnabledExtensionNames = extensionToEnable.data();
    createInfo.enabledExtensionCount = (uint32_t)extensionToEnable.size();

    VkInstance instance{ 0 };
    VK_CHECK( vkCreateInstance(&createInfo, nullptr, &instance) );

    return instance;
}

VkDebugReportCallbackEXT registerDebugCallback(VkInstance instance) {
    auto callbackFunction = [](VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, 
                                int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData) ->VkBool32
    {
        const auto error = flags & VK_DEBUG_REPORT_ERROR_BIT_EXT;
        const auto warning = flags & VK_DEBUG_REPORT_WARNING_BIT_EXT;
        const auto perfomanceWarning = flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;

        if (error) {
            printf("%s : %s\n", "ERROR", pMessage);
            //__debugbreak();
        }
        else if (warning) {
            printf("%s : %s\n", "WARNING", pMessage);
            //__debugbreak();
        }
        else if (perfomanceWarning) {
            printf("%s : %s\n", "PERFOMANCE_WARNING", pMessage);
            //__debugbreak();
        }

        return VK_FALSE;
    };

    VkDebugReportCallbackCreateInfoEXT createInfo{ VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT };
    createInfo.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
    createInfo.pfnCallback = callbackFunction;
    
    auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

    VkDebugReportCallbackEXT callback{ 0 };
    VK_CHECK( vkCreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) );

    return callback;
}

bool supportPresentation(VkPhysicalDevice physicalDevice, uint32_t familyIndex) {
    if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
        return vkGetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, familyIndex);
    }
    else {
        return false;
    }
}

uint32_t getGraphicsQueueFamily(VkPhysicalDevice physicalDevice) {
    uint32_t queuesCount{ 0 };
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, nullptr);

    std::vector<VkQueueFamilyProperties> queues(queuesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queuesCount, queues.data());

    for (uint32_t i = 0; i < queuesCount; i++) {
        if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { return i; }
    }

    return VK_QUEUE_FAMILY_IGNORED;
}

VkPhysicalDevice selectPhysicalDevice(VkInstance instance) {
    uint32_t physicalDeviceCount{ 0 };
    VK_CHECK( vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr) );

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    VK_CHECK( vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data()) );

    auto pickPhysicalDevice = [&physicalDevices]()->VkPhysicalDevice {
        const auto isSuitable = [](const VkPhysicalDevice physicalDevice) {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(physicalDevice, &properties);
            // properties.limits ---> VkPhysicalDeviceLimits (check this for some parameters)

            //VkPhysicalDeviceFeatures deviceFeatures;  // features can be also checked
            //vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

            const auto familyIndex = getGraphicsQueueFamily(physicalDevice);

            if (familyIndex == VK_QUEUE_FAMILY_IGNORED) { return false; }
            if (!supportPresentation(physicalDevice, familyIndex)) { return false; };

            return properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
        };

        const auto it = std::find_if(physicalDevices.cbegin(), physicalDevices.cend(), isSuitable);

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
    };

    return pickPhysicalDevice();
}

void fillDeviceNeededExtensions(VkPhysicalDevice physicalDevice, std::vector<const char*>& extensionsToEnable) {
    uint32_t extensionCount{ 0 };
    VK_CHECK( vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr) );

    std::vector<VkExtensionProperties> extensionProperties(extensionCount);
    VK_CHECK( vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensionProperties.data()));

    const bool isAvailableSwapchain = pushExtensionIfAvailable(extensionsToEnable, extensionProperties, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

VkDevice createDevice(VkPhysicalDevice physicalDevice, uint32_t familyIndex) {
    float queuePriority{ 1.f };

    VkDeviceQueueCreateInfo queueInfo{ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueInfo.queueFamilyIndex = familyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    std::vector<const char*> extensionsToEnable;
    fillDeviceNeededExtensions(physicalDevice, extensionsToEnable);

    VkDeviceCreateInfo createInfo{ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueInfo;
    createInfo.enabledExtensionCount = extensionsToEnable.size();
    createInfo.ppEnabledExtensionNames = extensionsToEnable.data();

    VkDevice device{ 0 };
    VK_CHECK( vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) );

    return device;
}

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow* window) {
    if constexpr (VK_USE_PLATFORM_WIN32_KHR) {
        VkWin32SurfaceCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
        createInfo.hinstance = GetModuleHandle(nullptr);
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
    uint32_t formatCount{ 0 };
    VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr) );

    std::vector<VkSurfaceFormatKHR> formats(formatCount);
    VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data()) );

    if (formatCount == 1 && formats[0].format == VK_FORMAT_UNDEFINED) { return VK_FORMAT_R8G8B8A8_UNORM; }

    /*
    const auto it = std::find_if(formats.cbegin(), formats.cend(), [](const VkSurfaceFormatKHR surfaceFormat) {
        return surfaceFormat.format == VK_FORMAT_A2R10G10B10_UNORM_PACK32 || surfaceFormat.format == VK_FORMAT_A2B10G10R10_UNORM_PACK32;
    });

    if (it != formats.cend()) { return (*it).format; }
    */
    {
        const auto it = std::find_if(formats.cbegin(), formats.cend(), [](const VkSurfaceFormatKHR surfaceFormat) {
            return surfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM || surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM;
        });

        if (it != formats.cend()) { return (*it).format; }
    }

    return formats[0].format;
}

VkPresentModeKHR getPresentMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    uint32_t presentModeCount{ 0 };
    VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr) );

    std::vector<VkPresentModeKHR> presentModes(presentModeCount);
    VK_CHECK( vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data()) );

    const auto it = std::find_if(presentModes.cbegin(), presentModes.cend(), [](const VkPresentModeKHR mode) {
        return mode == VK_PRESENT_MODE_MAILBOX_KHR;
    });

    if (it != presentModes.cend()) {
        return VK_PRESENT_MODE_MAILBOX_KHR;
    }
    
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkSwapchainKHR createSwapchain(VkDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR surfaceCaps, VkPresentModeKHR presentMode, VkFormat format, VkExtent2D extent, const uint32_t* familyIndex) {
    VkCompositeAlphaFlagBitsKHR surfaceComposite = [&surfaceCaps]() {
        if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) { 
            return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR };
        }
        else if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR) {
            return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR };
        }
        else if (surfaceCaps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) {
            return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR };
        }
        else { 
            return VkCompositeAlphaFlagBitsKHR{ VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR }; 
        }
    }();

    VkSwapchainCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    createInfo.surface = surface;
    createInfo.minImageCount = std::max(2u, surfaceCaps.minImageCount);
    createInfo.imageFormat = format;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = familyIndex;
    createInfo.presentMode = presentMode;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    createInfo.compositeAlpha = surfaceComposite;

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
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    VkImageView view{ 0 };

    VK_CHECK( vkCreateImageView(device, &createInfo, nullptr, &view) );

    return view;
}

VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView imageView, VkExtent2D extent) {
    VkFramebufferCreateInfo createInfo{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
    createInfo.renderPass = renderPass;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &imageView;
    createInfo.width = extent.width;
    createInfo.height = extent.height;
    createInfo.layers = 1;

    VkFramebuffer framebuffer{ 0 };
    VK_CHECK( vkCreateFramebuffer(device, &createInfo, nullptr, &framebuffer) );

    return framebuffer;
}

void fillImageViewsAndFramebuffers(VkDevice device, VkSwapchainKHR swapchain, VkRenderPass renderPass, VkFormat format, VkExtent2D extent, 
    std::vector<VkImage>& images, std::vector<VkImageView>& imageViews, std::vector<VkFramebuffer>& framebuffers) 
{
    uint32_t swapchainImageCount{ 0 };
    VK_CHECK( vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, nullptr) );

    images.resize(swapchainImageCount);
    imageViews.resize(swapchainImageCount);
    framebuffers.resize(swapchainImageCount);

    VK_CHECK( vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, images.data()) );

    for (uint32_t i = 0; i < swapchainImageCount; i++) {
        imageViews[i] = createImageView(device, images[i], format);
    }

    for (uint32_t i = 0; i < swapchainImageCount; i++) {
        framebuffers[i] = createFramebuffer(device, renderPass, imageViews[i], extent);
    }
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

    VK_CHECK(vkCreatePipelineLayout(device, &createInfo, nullptr, &layout));

    return layout;
}

VkPipeline createGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, VkPipelineCache pipelineCache, 
    VkViewport viewport, VkRect2D scissor, VkShaderModule vertex, VkShaderModule fragment) 
{
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
    Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    const auto instance{ createVulkanInstance() };
    const auto debugCallback{ registerDebugCallback(instance) };
    const auto physicalDevice{ selectPhysicalDevice(instance) };
    const auto familyIndex{ getGraphicsQueueFamily(physicalDevice) };
    const auto device{ createDevice(physicalDevice, familyIndex) };
    const auto surface{ createSurface(instance, window.getWindow()) };

    VkBool32 presentSupported{ 0 };
    VK_CHECK( vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &presentSupported) );

    VkSurfaceCapabilitiesKHR surfaceCaps{ 0 };
    VK_CHECK( vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCaps) );

    const VkExtent2D windowSize{ window.getWidth() , window.getHeight() };
    const VkPresentModeKHR presentMode{ getPresentMode(physicalDevice, surface) };

    const VkViewport viewport{ 0.f , (float)window.getHeight(), (float)window.getWidth(), -(float)window.getHeight(), 0.f, 1.f };
    const VkRect2D scissor{ {0, 0 }, windowSize };

    const auto swapchainFormat{ getSwapchainFormat(physicalDevice, surface) };
    const auto swapchain{ createSwapchain(device, surface, surfaceCaps, presentMode, swapchainFormat, windowSize, &familyIndex) };
    const auto releaseSemaphore{ createSemaphore(device) };
    const auto acquireSemaphore{ createSemaphore(device) };
    const auto renderPass{ createRenderPass(device, swapchainFormat) };
    const auto commandPool{ createCommandPool(device, familyIndex) };
    const auto commandBuffer{ createCommandBuffer(device, commandPool) };
    const auto triangleVertShader{ loadShader(device, "resources/triangle.vert.spv") };
    const auto triangleFragShader{ loadShader(device, "resources/triangle.frag.spv") };
    VkPipelineCache pipelineCache{ 0 }; // critical for perfomance
    const auto triangleLayout{ createPipelineLayout(device) };
    const auto trianglePipeline{ createGraphicsPipeline(device, renderPass, triangleLayout, pipelineCache, viewport, scissor, triangleVertShader, triangleFragShader) };

    VkQueue queue{ 0 };
    vkGetDeviceQueue(device, familyIndex, 0, &queue);

    std::vector<VkImage> swapchainImages;
    std::vector<VkImageView> swapchainImageViews;
    std::vector<VkFramebuffer> swapchainFramebuffers;
    fillImageViewsAndFramebuffers(device, swapchain, renderPass, swapchainFormat, windowSize, swapchainImages, swapchainImageViews, swapchainFramebuffers);

    while (window.shouldClose()) {
        uint32_t imageIndex{ 0 };
        VK_CHECK( vkAcquireNextImageKHR(device, swapchain, ~0ull, acquireSemaphore, VK_NULL_HANDLE, &imageIndex) );

        VK_CHECK( vkResetCommandPool(device, commandPool, 0) );

        VkCommandBufferBeginInfo beginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK( vkBeginCommandBuffer(commandBuffer, &beginInfo) );
        
        const auto renderBeginBarrier{ pipelineBarrierImage(swapchainImages[imageIndex], 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &renderBeginBarrier);

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

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, trianglePipeline);
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        const auto renderEndBarrier{ pipelineBarrierImage(swapchainImages[imageIndex], VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_DEPENDENCY_BY_REGION_BIT, 0, nullptr, 0, nullptr, 1, &renderEndBarrier);

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

    VK_CHECK( vkDeviceWaitIdle(device) );

    std::for_each(swapchainImageViews.cbegin(), swapchainImageViews.cend(), [&device](const VkImageView imageView) {
        vkDestroyImageView(device, imageView, nullptr);
    });

    std::for_each(swapchainFramebuffers.cbegin(), swapchainFramebuffers.cend(), [&device](const VkFramebuffer framebuffer) {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    });

    vkDestroyPipeline(device, trianglePipeline, nullptr);

    vkDestroyPipelineLayout(device, triangleLayout, nullptr);

    vkDestroyPipelineCache(device, pipelineCache, nullptr);

    vkDestroyShaderModule(device, triangleVertShader, nullptr);
    vkDestroyShaderModule(device, triangleFragShader, nullptr);

    vkDestroyCommandPool(device, commandPool, nullptr);

    vkDestroyRenderPass(device, renderPass, nullptr);

    vkDestroySemaphore(device, releaseSemaphore, nullptr);
    vkDestroySemaphore(device, acquireSemaphore, nullptr);

    vkDestroySwapchainKHR(device, swapchain, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);

    vkDestroyDevice(device, nullptr);

    auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    vkDestroyDebugReportCallbackEXT(instance, debugCallback, nullptr);

    vkDestroyInstance(instance, nullptr);

    window.terminate();

    exit(EXIT_SUCCESS);
}
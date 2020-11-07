
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

VkSwapchainKHR createSwapchain(VkDevice device, VkSurfaceKHR surface, int32_t width, int32_t height, const uint32_t* familyIndex) {

    VkSwapchainCreateInfoKHR createInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    createInfo.surface = surface;
    createInfo.minImageCount = 2;
    createInfo.imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
    createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    createInfo.imageExtent.width = width;
    createInfo.imageExtent.height = height;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.queueFamilyIndexCount = 1;
    createInfo.pQueueFamilyIndices = familyIndex;
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

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

int main(void) {
    Window window{};
    window.initialize("MAREngine Vulkan Renderer", 1200, 800);

    uint32_t familyIndex{ 0 }; // this may produce validation error
    const auto instance = createVulkanInstance();
    const auto physicalDevice = createPhysicalDevice(instance);
    const auto device = createDevice(physicalDevice, familyIndex);
    const auto surface = createSurface(instance, window.getWindow());
    const auto swapchain = createSwapchain(device, surface, window.getWidth(), window.getHeight(), &familyIndex);
    const auto semaphore = createSemaphore(device); // check 1:40:20

    while (window.shouldClose()) {
        window.clear();



        window.pollEvents();
    }

    window.terminate();

    exit(EXIT_SUCCESS);
}
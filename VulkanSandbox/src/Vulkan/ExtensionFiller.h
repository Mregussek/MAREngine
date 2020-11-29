

#ifndef MAR_ENGINE_PLATFORM_VULKAN_EXTENSION_FILLER_H
#define MAR_ENGINE_PLATFORM_VULKAN_EXTENSION_FILLER_H


#include "../../VulkanInclude.h"


namespace mar {


	bool pushExtensionIfAvailable(std::vector<const char*>& extensionsToEnable, std::vector<VkExtensionProperties>& allExtensions, const char* extensionProperty);

	bool pushLayerIfAvailable(std::vector<const char*>& layersToEnable, std::vector<VkLayerProperties>& allLayers, const char* layerProperty);

}


#endif // !MAR_ENGINE_PLATFORM_VULKAN_EXTENSION_FILLER_H

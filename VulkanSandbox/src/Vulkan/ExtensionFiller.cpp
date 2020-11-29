

#include "ExtensionFiller.h"
#include "../../VulkanLogging.h"


namespace mar {


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

    bool pushLayerIfAvailable(std::vector<const char*>& layersToEnable, std::vector<VkLayerProperties>& allLayers, const char* layerProperty) {
        const auto availableLayer = [&layerProperty](const VkLayerProperties property) {
            return std::strcmp(property.layerName, layerProperty) == 0;
        };

        if (std::find_if(allLayers.begin(), allLayers.end(), availableLayer) != allLayers.end()) {
            layersToEnable.emplace_back(layerProperty);
            return true;
        }

        return false;
    }


}

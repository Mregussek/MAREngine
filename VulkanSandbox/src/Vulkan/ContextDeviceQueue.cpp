
#include "ContextVulkan.h"
#include "../../VulkanLogging.h"


namespace mar {


	void ContextVulkan::createDeviceQueue() {
		vkGetDeviceQueue(m_device, m_familyIndex, 0, &m_queue);

		VkSemaphoreCreateInfo createInfo{ VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		VK_CHECK( vkCreateSemaphore(m_device, &createInfo, nullptr, &m_acquireSemaphore) );
		VK_CHECK( vkCreateSemaphore(m_device, &createInfo, nullptr, &m_releaseSemaphore) );
	}

	void ContextVulkan::closeDeviceQueue() const {
		vkDestroySemaphore(m_device, m_releaseSemaphore, nullptr);
		vkDestroySemaphore(m_device, m_acquireSemaphore, nullptr);
	}


}

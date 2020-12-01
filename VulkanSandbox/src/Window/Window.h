
#ifndef MAR_ENGINE_WINDOW_VULKAN_H
#define MAR_ENGINE_WINDOW_VULKAN_H


#include "../../VulkanInclude.h"


namespace mar {


	class Window {
	public:

		static Window* Instance();


		void initialize(const char* name, int32_t width, int32_t height);

		void pollEvents();

		void terminate() const;

		bool shouldClose() const;

		GLFWwindow* getWindow() const;

		int32_t getWidth() const;

		int32_t getHeight() const;

	private:
		

		static Window* s_instance;


		const char* m_name{ "Vulkan Window" };
		int32_t m_width{ 1200 };
		int32_t m_height{ 800 };
		GLFWwindow* m_window{ nullptr };

	};


}


#endif // !MAR_ENGINE_WINDOW_VULKAN_H

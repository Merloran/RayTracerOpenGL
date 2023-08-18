#include "display_manager.hpp"

#include <GLFW/glfw3.h>


SDisplayManager& SDisplayManager::get()
{
	static SDisplayManager instance;
	return instance;
}

Void SDisplayManager::startup()
{
	SPDLOG_INFO("Display Manager startup.");
	if (!glfwInit())
	{
		SPDLOG_ERROR("Failed to initialize GLFW!");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(windowSize.x, windowSize.y, name.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(window);
}

const glm::ivec2& SDisplayManager::get_framebuffer_size()
{
	if (window == nullptr)
	{
		SPDLOG_ERROR("Window is null!");
		return { -1, -1 };
	}
	glfwGetFramebufferSize(window, &framebufferSize.x, &framebufferSize.y);
	return framebufferSize;
}

const glm::ivec2& SDisplayManager::get_window_size()
{
	if (window == nullptr)
	{
		SPDLOG_ERROR("Window is null!");
		return { -1, -1 };
	}
	glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
	return windowSize;
}

Void SDisplayManager::update()
{
	glfwPollEvents();
}

Void SDisplayManager::close_window()
{
	glfwSetWindowShouldClose(window, GL_TRUE);
}

Bool SDisplayManager::should_window_close()
{
	if (window == nullptr)
	{
		SPDLOG_ERROR("Window is null!");
		return true;
	}

	return glfwWindowShouldClose(window);
}

Void SDisplayManager::shutdown()
{
	SPDLOG_INFO("Display Manager shutdown.");
	glfwDestroyWindow(window);
	glfwTerminate();
	window = nullptr;
}

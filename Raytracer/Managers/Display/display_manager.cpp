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
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(windowSize.x, windowSize.y, name.c_str(), nullptr, nullptr);

	if (!window)
	{
		SPDLOG_ERROR("Failed to create window!");
		return;
	}
	glfwMakeContextCurrent(window);
}

const glm::ivec2& SDisplayManager::get_frame_buffer_size()
{
	if (window == nullptr)
	{
		SPDLOG_ERROR("Window is null!");
		return { -1, -1 };
	}
	glfwGetFramebufferSize(window, &frameBufferSize.x, &frameBufferSize.y);
	return frameBufferSize;
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

Float32 SDisplayManager::get_aspect_ratio() const
{
	if (windowSize.y == 0)
	{
		return 0.0f;
	}

	return Float32(windowSize.x) / Float32(windowSize.y);
}

Void SDisplayManager::poll_events()
{
	glfwPollEvents();
	glfwGetWindowSize(window, &windowSize.x, &windowSize.y);
	glfwGetFramebufferSize(window, &frameBufferSize.x, &frameBufferSize.y);
	glViewport(0, 0, windowSize.x, windowSize.y);
}

Void SDisplayManager::make_context_current()
{
	glfwMakeContextCurrent(window);
}

Void SDisplayManager::swap_buffers()
{
	glfwSwapBuffers(window);
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

GLFWwindow& SDisplayManager::get_window()
{
	return *window;
}

Void SDisplayManager::shutdown()
{
	SPDLOG_INFO("Display Manager shutdown.");
	glfwDestroyWindow(window);
	glfwTerminate();
	window = nullptr;
}

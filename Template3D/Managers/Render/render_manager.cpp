#include "render_manager.hpp"
#include "../Display/display_manager.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

SRenderManager& SRenderManager::get()
{
	static SRenderManager instance;
	return instance;
}

Void SRenderManager::startup()
{
	SPDLOG_INFO("Render Manager startup.");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		SPDLOG_ERROR("Failed to initialize OpenGL loader!");
		SDisplayManager::get().close_window();
		return;
	}
}

Void SRenderManager::shutdown()
{
	SPDLOG_INFO("Render Manager shutdown.");
}

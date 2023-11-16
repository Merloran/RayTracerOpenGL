#include "render_manager.hpp"

#include "../Display/display_manager.hpp"
#include "../Resource/resource_manager.hpp"
#include "../Resource/Common/model.hpp"
#include "../Resource/Common/mesh.hpp"
#include "../Resource/Common/texture.hpp"
#include "../Resource/Common/material.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		SPDLOG_ERROR("Failed to initialize OpenGL loader!");
		SDisplayManager::get().close_window();
		return;
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	const SDisplayManager& displayManager = SDisplayManager::get();

	ImGui_ImplGlfw_InitForOpenGL(&displayManager.get_window(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	// Setup style
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

Void SRenderManager::draw_model(const Model& model)
{
	SResourceManager& resourceManager = SResourceManager::get();

	for (Int32 i = 0; i < model.meshes.size(); i++)
	{
		Mesh &mesh = resourceManager.get_mesh_by_handle(model.meshes[i]);
		Material &material = resourceManager.get_material_by_handle(model.materials[i]);

		Handle<Texture> *textureHandle = &material.albedo;
		for (Int32 j = 0; j < sizeof(Material) / sizeof(Handle<Texture>); ++j, ++textureHandle)
		{
			if (*textureHandle != Handle<Texture>::sNone)
			{
				const Texture &texture = resourceManager.get_texture_by_handle(*textureHandle);
				std::string type(magic_enum::enum_name(texture.type));
				// shader.set_int(type, j);
				glActiveTexture(GL_TEXTURE0 + j);
				glBindTexture(GL_TEXTURE_2D, texture.gpuId);
			}
		}

		glBindVertexArray(mesh.gpuIds[0]);
		glDrawElements(GL_TRIANGLES, mesh.indexes.size(), GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
    
}

Void SRenderManager::shutdown()
{
	SPDLOG_INFO("Render Manager shutdown.");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

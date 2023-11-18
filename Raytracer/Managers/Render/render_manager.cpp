#include "render_manager.hpp"

#include "Common/shader.hpp"
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

	SDisplayManager& displayManager = SDisplayManager::get();

	ImGui_ImplGlfw_InitForOpenGL(&displayManager.get_window(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	// Setup style
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	Int32 flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gl_debug, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

Void SRenderManager::draw_model(const Model& model, Shader& shader)
{
	SResourceManager& resourceManager = SResourceManager::get();

	for (Int32 i = 0; i < model.meshes.size(); i++)
	{
		Mesh &mesh = resourceManager.get_mesh_by_handle(model.meshes[i]);
		Material &material = resourceManager.get_material_by_handle(model.materials[i]);

		Handle<Texture> *textureHandle = &material.albedo;
		const Int32 texturesCount = (offsetof(Material, opacity) - offsetof(Material, albedo)) / sizeof(Handle<Texture>);
		for (Int32 j = 0; j < texturesCount; ++j, ++textureHandle)
		{
			if (*textureHandle != Handle<Texture>::sNone)
			{
				const Texture &texture = resourceManager.get_texture_by_handle(*textureHandle);
				std::string type(magic_enum::enum_name(texture.type));
				shader.set_int(type, j);
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

Void SRenderManager::gl_debug(UInt32 source, UInt32 type, UInt32 id, UInt32 severity,
							  Int32 length, const Char* message, const Void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
	{
		return;
	}

	SPDLOG_INFO("Debug message ({}): {}", id, message);

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:
		{
			SPDLOG_INFO("Source: API");
			break;
		}
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		{
			SPDLOG_INFO("Source: Window System");
			break;
		}
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
		{
			SPDLOG_INFO("Source: Shader Compiler");
			break;
		}
		case GL_DEBUG_SOURCE_THIRD_PARTY:
		{
			SPDLOG_INFO("Source: Third Party");
			break;
		}
		case GL_DEBUG_SOURCE_APPLICATION:
		{
			SPDLOG_INFO("Source: Application");
			break;
		}
		case GL_DEBUG_SOURCE_OTHER:
		{
			SPDLOG_INFO("Source: Other");
			break;
		}
		default:
		{
			SPDLOG_ERROR("NOT SUPPORTED");
			break;
		}
	}

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:
		{
			SPDLOG_INFO("Type: Error");
			break;
		}
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		{
			SPDLOG_INFO("Type: Deprecated Behaviour");
			break;
		}
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		{
			SPDLOG_INFO("Type: Undefined Behaviour");
			break;
		}
		case GL_DEBUG_TYPE_PORTABILITY:
		{
			SPDLOG_INFO("Type: Portability");
			break;
		}
		case GL_DEBUG_TYPE_PERFORMANCE:
		{
			SPDLOG_INFO("Type: Performance");
			break;
		}
		case GL_DEBUG_TYPE_MARKER:
		{
			SPDLOG_INFO("Type: Marker");
			break;
		}
		case GL_DEBUG_TYPE_PUSH_GROUP:
		{
			SPDLOG_INFO("Type: Push Group");
			break;
		}
		case GL_DEBUG_TYPE_POP_GROUP:
		{
			SPDLOG_INFO("Type: Pop Group");
			break;
		}
		case GL_DEBUG_TYPE_OTHER:
		{
			SPDLOG_INFO("Type: Other");
			break;
		}
		default:
		{
			SPDLOG_ERROR("NOT SUPPORTED");
			break;
		}
	}

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:
		{
			SPDLOG_INFO("Severity: high");
			break;
		}
		case GL_DEBUG_SEVERITY_MEDIUM:
		{
			SPDLOG_INFO("Severity: medium");
			break;
		}
		case GL_DEBUG_SEVERITY_LOW:
		{
			SPDLOG_INFO("Severity: low");
			break;
		}
		case GL_DEBUG_SEVERITY_NOTIFICATION:
		{
			SPDLOG_INFO("Severity: notification");
			break;
		}
		default:
		{
			SPDLOG_ERROR("NOT SUPPORTED");
			break;
		}
	}
}

Void SRenderManager::shutdown()
{
	SPDLOG_INFO("Render Manager shutdown.");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

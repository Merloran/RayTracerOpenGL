#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "Managers/Resource/resource_manager.hpp"
#include "Managers/Display/display_manager.hpp"
#include "Managers/Render/render_manager.hpp"
#include "Managers/Render/Common/camera.hpp"
#include "Managers/Render/Common/shader.hpp"
#include "Managers/Resource/Common/model.hpp"


Void camera_gui(Camera& camera)
{
	ImGui::Begin("Camera settings");
	ImGui::DragFloat("Sensitivity", &camera.sensitivity, 0.2f, 0.f, 100.0f);
	ImGui::DragFloat("Speed", &camera.speed, 1.0f, 0.f, 100.0f);
	ImGui::DragFloat("Near", &camera.near, 0.001f, 0.001f, 1.0f);
	ImGui::DragFloat("Far", &camera.far, 1.0f, 1.0f, 5000.0f);
	ImGui::Text("FPS: %.2f, %.2fms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();
}


Int32 main()
{
	SResourceManager& resourceManager = SResourceManager::get();
	SDisplayManager& displayManager = SDisplayManager::get();
	SRenderManager& renderManager = SRenderManager::get();

	displayManager.startup();
	renderManager.startup();
	resourceManager.startup();
	// resourceManager.load_gltf_asset("Resources/Assets/Bistro/Bistro.gltf");
	resourceManager.generate_opengl_resources();
	Shader diffuse;
	Camera camera;

	diffuse.create("Resources/Shaders/Vertex.vert", "Resources/Shaders/Fragment.frag");
	camera.initialize(glm::vec3(0.0f));

	Float32 lastFrame = 0.0f;
	while (!displayManager.should_window_close())
	{
		Float32 currentFrame = glfwGetTime();
		Float32 deltaTimeMs = currentFrame - lastFrame;
		lastFrame = currentFrame;

		displayManager.poll_events();
		camera.get_input(deltaTimeMs);

		glClearColor(0.31f, 0.22f, 0.16f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		camera_gui(camera);

		ImGui::Render();

		displayManager.make_context_current();

		diffuse.use();
		glm::mat4 view = camera.get_view();
		glm::mat4 proj = camera.get_projection(displayManager.get_aspect_ratio());
		glm::mat4 model = glm::mat4(1.0f);
		diffuse.set_mat4("viewProjection", proj * view);
		diffuse.set_mat4("model", model);
		
		for (const Model& asset : resourceManager.get_models())
		{
			renderManager.draw_model(asset, diffuse);
		}
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		displayManager.swap_buffers();
	}
	
	resourceManager.shutdown();
	renderManager.shutdown();
	displayManager.shutdown();
	return 0;
}
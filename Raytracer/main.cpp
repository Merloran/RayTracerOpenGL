#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Managers/Resource/resource_manager.hpp"
#include "Managers/Display/display_manager.hpp"
#include "Managers/Render/render_manager.hpp"
#include "Managers/Render/Common/camera.hpp"
#include "Managers/Render/Common/shader.hpp"
#include "Managers/Resource/Common/model.hpp"
#include "Managers/Raytrace/raytrace_manager.hpp"


Void camera_gui(Camera& camera)
{
	if (ImGui::Begin("Camera settings"))
	{
		glm::vec3 position = camera.get_position();
		glm::vec2 viewBounds = camera.get_view_bounds();
		Float32 sensitivity = camera.get_sensitivity();
		Float32 speed = camera.get_speed();
		Float32 fov = camera.get_fov();

		ImGui::DragFloat3("Position", &position[0], 0.1f, -10.0f, 10.0f);
		ImGui::DragFloat("Sensitivity", &sensitivity, 0.2f, 0.f, 100.0f);
		ImGui::DragFloat("Speed", &speed, 1.0f, 0.f, 100.0f);
		ImGui::DragFloat("Fov", &fov, 1.0f, -180.0f, 180.0f);
		ImGui::DragFloat("Near", &viewBounds.x, 0.001f, 0.001f, 1.0f);
		ImGui::DragFloat("Far", &viewBounds.y, 1.0f, 1.0f, 5000.0f);

		camera.set_position(position);
		camera.set_view_bounds(viewBounds);
		camera.set_sensitivity(sensitivity);
		camera.set_speed(speed);
		camera.set_fov(fov);
	}
	ImGui::End();
	
}

Void info_gui()
{
	if (ImGui::Begin("Render info"))
	{
		SRaytraceManager& raytraceManager = SRaytraceManager::get();
		ImGui::SliderInt("Bounces Count", &raytraceManager.maxBouncesCount, 1, 32);
		ImGui::Text("FPS: %.2f, %.2fms", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
		ImGui::Text("Accumulated frames: %d", raytraceManager.get_frame_count());
	}
	ImGui::End();
}


Int32 main()
{
	SResourceManager& resourceManager = SResourceManager::get();
	SDisplayManager& displayManager = SDisplayManager::get();
	SRenderManager& renderManager = SRenderManager::get();
	SRaytraceManager& raytraceManager = SRaytraceManager::get();

	displayManager.startup();
	renderManager.startup();
	resourceManager.startup();
	// resourceManager.load_gltf_asset(resourceManager.ASSETS_PATH + "Cube/Cube.gltf");
	// resourceManager.generate_opengl_textures();

	resourceManager.load_gltf_asset("Resources/Assets/Main.1_Sponza/NewSponza_Main_glTF_002.gltf");
	resourceManager.generate_opengl_resources();
	raytraceManager.startup();
	Shader diffuse;
	Camera camera;
	diffuse.create("Resources/Shaders/Vertex.vert", "Resources/Shaders/Fragment.frag");
	camera.initialize(glm::vec3(0.0f, 0.0f, 5.0f));

	Float32 lastFrame = 0.0f;
	while (!displayManager.should_window_close())
	{
		Float32 currentFrame = glfwGetTime();
		Float32 deltaTimeMs = currentFrame - lastFrame;
		lastFrame = currentFrame;

		displayManager.poll_events();
		camera.catch_input(deltaTimeMs);
		glm::vec3 color = raytraceManager.get_background_color();
		glClearColor(color.x, color.y, color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		camera_gui(camera);
		info_gui();

		ImGui::Render();

		displayManager.make_context_current();

		raytraceManager.update(camera, deltaTimeMs);

		// diffuse.use();
		// glm::mat4 view = camera.get_view();
		// glm::mat4 proj = camera.get_projection(displayManager.get_aspect_ratio());
		// glm::mat4 model = glm::mat4(1.0f);
		// diffuse.set_mat4("viewProjection", proj * view);
		// diffuse.set_mat4("model", model);
		//
		// for (const Model& asset : resourceManager.get_models())
		// {
		// 	renderManager.draw_model(asset, diffuse);
		// }


		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		displayManager.swap_buffers();
	}
	
	raytraceManager.shutdown();
	resourceManager.shutdown();
	renderManager.shutdown();
	displayManager.shutdown();
	return 0;
}
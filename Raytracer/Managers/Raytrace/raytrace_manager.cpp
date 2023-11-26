#include "raytrace_manager.hpp"

#include <glad/glad.h>

#include "../Render/Common/camera.hpp"
#include "../Render/render_manager.hpp"
#include "../Display/display_manager.hpp"

SRaytraceManager& SRaytraceManager::get()
{
	static SRaytraceManager instance;
	return instance;
}

Void SRaytraceManager::startup()
{
	triangle.create("Resources/Shaders/Triangle.comp");
	screen.create("Resources/Shaders/Screen.vert", "Resources/Shaders/Screen.frag");
	screen.use();
	screen.set_int("image", 0);

	positions.push_back(glm::vec4( 0.0f,  1.0f, 0.0f, 1.0f));
	positions.push_back(glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f));
	positions.push_back(glm::vec4( 1.0f, -1.0f, 0.0f, 1.0f));

	indexes.push_back(0);
	indexes.push_back(1);
	indexes.push_back(2);

    glGenBuffers(4, &ssbo[0]);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::vec4), &positions[0], GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, indexes.size() * sizeof(UInt32), &indexes[0], GL_DYNAMIC_DRAW);

    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo[2]);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, indexes.size() * sizeof(UInt32), &indexes[0], GL_DYNAMIC_DRAW);
    //
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo[3]);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, indexes.size() * sizeof(UInt32), &indexes[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindVertexArray(0);

	SDisplayManager& displayManager = SDisplayManager::get();
	const glm::ivec2& size = displayManager.get_window_size();
	create_opengl_texture(screenTexture, size);
	glBindImageTexture(0, screenTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

}


Void SRaytraceManager::update(Camera &camera)
{
	SDisplayManager &displayManager = SDisplayManager::get();
	SRenderManager &renderManager = SRenderManager::get();

	const glm::ivec2& size = displayManager.get_window_size();
	const Float32 theta = glm::radians(camera.fov);
	const Float32 h = glm::tan(theta * 0.5f);
	glm::ivec2 viewportSize;
	viewportSize.y = 2.0f * theta;
	viewportSize.x = viewportSize.y * displayManager.get_aspect_ratio();

	const glm::vec3 viewportU = Float32(viewportSize.x) * camera.get_right();
	const glm::vec3 viewportV = Float32(viewportSize.y) * camera.get_up();

	pixelDeltaU = viewportU / Float32(size.x);
	pixelDeltaV = viewportV / Float32(size.y);
	originPixel = camera.position + camera.get_forward() + (pixelDeltaU - viewportU + pixelDeltaV - viewportV) * 0.5f;

	triangle.use();
	triangle.set_vec3("cameraPosition", camera.position);
	triangle.set_vec3("originPixel", originPixel);
	triangle.set_vec3("pixelDeltaU", pixelDeltaU);
	triangle.set_vec3("pixelDeltaV", pixelDeltaV);
	triangle.set_ivec2("imageSize", size);
	triangle.set_vec2("viewBounds", camera.near, camera.far);
	
	glDispatchCompute(Int32(size.x / 16), Int32(size.y / 16), 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	screen.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	renderManager.draw_quad();
}

Void SRaytraceManager::create_opengl_texture(UInt32& texture, const glm::ivec2& size)
{
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Void SRaytraceManager::shutdown()
{
	glDeleteBuffers(2, ssbo);
	glDeleteTextures(1, &screenTexture);
}

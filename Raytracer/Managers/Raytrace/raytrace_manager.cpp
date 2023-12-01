#include "raytrace_manager.hpp"

#include <glad/glad.h>

#include "../Render/Common/camera.hpp"
#include "../Render/render_manager.hpp"
#include "../Display/display_manager.hpp"
#include "../Resource/resource_manager.hpp"
#include "../Resource/Common/model.hpp"
#include "../Resource/Common/handle.hpp"
#include "../Resource/Common/material.hpp"
#include "../Resource/Common/mesh.hpp"
#include "../Resource/Common/texture.hpp"

SRaytraceManager& SRaytraceManager::get()
{
	static SRaytraceManager instance;
	return instance;
}

Void SRaytraceManager::startup()
{
	SPDLOG_INFO("Raytrace Manager startup.");
	SResourceManager& resourceManager = SResourceManager::get();
	triangle.create("Resources/Shaders/Triangle.comp");
	screen.create("Resources/Shaders/Screen.vert", "Resources/Shaders/Screen.frag");
	screen.use();
	screen.set_int("image", 0);

	textures.reserve(resourceManager.get_textures().size());
	for (const Texture& texture : resourceManager.get_textures())
	{
		textures.emplace_back(texture.bindlessId);
	}

	materials.reserve(resourceManager.get_materials().size());
	for (const Material& material : resourceManager.get_materials())
	{
		GpuMaterial& gpuMaterial = materials.emplace_back();
		gpuMaterial.albedo	  = material.albedo.id;
		gpuMaterial.normal	  = material.normal.id;
		gpuMaterial.roughness = material.roughness.id;
		gpuMaterial.metalness = material.metalness.id;
		gpuMaterial.emission  = material.emission.id;
	}

	Int32 vertexesSize = 0;
	Int32 indexesSize = 0;

	const std::vector<Model>& models = resourceManager.get_models();
	for (const Model& model : models)
	{
		for (Int32 i = 0; i < model.meshes.size(); ++i)
		{
			const Mesh& mesh = resourceManager.get_mesh_by_handle(model.meshes[i]);
			vertexesSize += mesh.positions.size();
			indexesSize += mesh.indexes.size();
		}
	}
	positionsWithMaterial.reserve(vertexesSize);
	normals.reserve(vertexesSize);
	uvs.reserve(vertexesSize);
	indexes.reserve(indexesSize);

	for (const Model& model : models)
	{
		for (Int32 i = 0; i < model.meshes.size(); ++i)
		{
			const Mesh& mesh = resourceManager.get_mesh_by_handle(model.meshes[i]);
			for (const glm::vec3& position : mesh.positions)
			{
				Int32 materialIdInt = model.materials[i].id;
				Float32 materialId = *reinterpret_cast<Float32*>(&materialIdInt);
				positionsWithMaterial.emplace_back(position, materialId);
			}
		}
	}

	for (const Model& model : models)
	{
		for (const Handle<Mesh> meshHandle : model.meshes)
		{
			const Mesh& mesh = resourceManager.get_mesh_by_handle(meshHandle);
			for (const glm::vec3 &normal : mesh.normals)
			{
				normals.emplace_back(normal, 1.0f);
			}
		}
	}

	for (const Model& model : models)
	{
		for (const Handle<Mesh> meshHandle : model.meshes)
		{
			const Mesh& mesh = resourceManager.get_mesh_by_handle(meshHandle);
			for (const glm::vec2 &uv : mesh.uvs)
			{
				uvs.emplace_back(uv);
			}
		}
	}

	Int32 indexesOffset = 0;
	for (const Model& model : models)
	{
		for (Int32 i = 0; i < model.meshes.size(); ++i)
		{
			const Mesh& mesh = resourceManager.get_mesh_by_handle(model.meshes[i]);
			for (Int32 j = 0; j < mesh.indexes.size(); ++j)
			{
				indexes.emplace_back(mesh.indexes[j] + indexesOffset);
			}
			indexesOffset += mesh.indexes.size();
		}
	}
	
    glCreateBuffers(6, &ssbo[0]);

	//Positions
	glNamedBufferStorage(ssbo[0],
				 positionsWithMaterial.size()  * sizeof(glm::vec4),
				 positionsWithMaterial.data(), 
				 GL_DYNAMIC_STORAGE_BIT);

	//Normals
	glNamedBufferStorage(ssbo[1],
				 normals.size() * sizeof(glm::vec4), 
				 normals.data(),
				 GL_DYNAMIC_STORAGE_BIT);

	//UV
	glNamedBufferStorage(ssbo[2],
				 uvs.size() * sizeof(glm::vec2), 
				 uvs.data(),
				 GL_DYNAMIC_STORAGE_BIT);

	//Indexes
	glNamedBufferStorage(ssbo[3],
				 indexes.size() * sizeof(UInt32),
				 indexes.data(),
				 GL_DYNAMIC_STORAGE_BIT);

	//Textures
	glNamedBufferStorage(ssbo[4],
				 textures.size() * sizeof(UInt64),
				 textures.data(),
				 GL_DYNAMIC_STORAGE_BIT);

	//Materials
	glNamedBufferStorage(ssbo[5],
				 materials.size() * sizeof(GpuMaterial),
				 materials.data(),
				 GL_DYNAMIC_STORAGE_BIT);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo[1]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo[2]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo[3]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo[4]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo[5]);
}


Void SRaytraceManager::update(Camera &camera)
{
	SDisplayManager &displayManager = SDisplayManager::get();
	SRenderManager &renderManager = SRenderManager::get();

	const glm::ivec2& size = displayManager.get_window_size();
	if (imageSize != size)
	{
		imageSize = size;
		resize_opengl_texture(screenTextures[0], imageSize);
		resize_opengl_texture(screenTextures[1], imageSize);
		glBindImageTexture(0, screenTextures[0], 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}
	
	const Float32 theta = glm::radians(camera.fov);
	const Float32 h = glm::tan(theta * 0.5f);
	glm::vec2 viewportSize;
	viewportSize.y = 2.0f * h;
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
	triangle.set_int("trianglesCount", Int32(indexes.size() / 3));
	
	glDispatchCompute(Int32(size.x / 16), Int32(size.y / 16), 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	screen.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTextures[0]);
	renderManager.draw_quad();
}

Void SRaytraceManager::resize_opengl_texture(UInt32& texture, const glm::ivec2& size)
{
	if (texture)
	{
		glDeleteTextures(1, &texture);
	}
	
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Void SRaytraceManager::shutdown()
{
	glDeleteBuffers(6, ssbo);
	glDeleteTextures(2, screenTextures);
}

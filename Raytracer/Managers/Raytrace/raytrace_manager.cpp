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
#include "BVH/bvh_node.hpp"

SRaytraceManager& SRaytraceManager::get()
{
	static SRaytraceManager instance;
	return instance;
}

Void SRaytraceManager::startup()
{
	SPDLOG_INFO("Raytrace Manager startup.");
	SResourceManager& resourceManager = SResourceManager::get();
	rayGeneration.create("Resources/Shaders/RayGeneration.comp");
	rayTrace.create("Resources/Shaders/Triangle.comp");
	screen.create("Resources/Shaders/Screen.vert", "Resources/Shaders/Screen.frag");
	screen.use();
	screen.set_int("accumulated", 0);
	renderTime = 0.0f;
	maxBouncesCount = 0;
	backgroundColor = { 0.0f, 0.0f, 0.0f };//{ 0.0f, 0.71f, 0.71f };

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
	trianglesCount = Int32(indexesSize / 3);
	// Predicting emission triangles count
	emissionTriangles.reserve(glm::max(Int32(trianglesCount * 0.01), 1));

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
				const GpuMaterial& gpuMaterial = materials[*reinterpret_cast<Int32*>(&positionsWithMaterial[mesh.indexes[j] + indexesOffset].w)];
				if (j % 3 == 0 && gpuMaterial.emission != -1)
				{
					emissionTriangles.emplace_back(mesh.indexes[j] + indexesOffset);
				}
			}
			indexesOffset += mesh.positions.size();
		}
	}

	bvh.create_tree(positionsWithMaterial, indexes);
	
    glCreateBuffers(8, &ssbo[0]);

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

	//BVHNodes
	glNamedBufferStorage(ssbo[6],
				 bvh.hierarchy.size() * sizeof(BVHNode),
				 bvh.hierarchy.data(),
				 GL_DYNAMIC_STORAGE_BIT);

	//EmissionTriangles
	glNamedBufferStorage(ssbo[7],
				 emissionTriangles.size() * sizeof(UInt32),
				 emissionTriangles.data(),
				 GL_DYNAMIC_STORAGE_BIT);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo[1]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo[2]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo[3]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, ssbo[4]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, ssbo[5]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, ssbo[6]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, ssbo[7]);
}


Void SRaytraceManager::update(Camera &camera, Float32 deltaTime)
{
	SDisplayManager &displayManager = SDisplayManager::get();
	SRenderManager &renderManager = SRenderManager::get();

	const glm::ivec2& size = displayManager.get_window_size();

	renderTime += deltaTime;
	const Bool hasWindowResized = imageSize != size;
	const Bool hasCameraChanged = camera.has_changed();

	if (hasWindowResized)
	{
		imageSize = size;
		resize_opengl_texture(screenTexture, imageSize);
		resize_opengl_texture(directionTexture, imageSize);
		glBindImageTexture(0, directionTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
		glBindImageTexture(1, screenTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}

	if (hasWindowResized || hasCameraChanged)
	{
		generate_rays(camera);
		renderTime = 0.0f;
	}
	rayTrace.use();
	rayTrace.set_vec3("backgroundColor", backgroundColor);
	rayTrace.set_vec3("cameraPosition", camera.get_position());
	rayTrace.set_vec3("pixelDeltaU", pixelDeltaU);
	rayTrace.set_vec3("pixelDeltaV", pixelDeltaV);
	rayTrace.set_ivec2("imageSize", imageSize);
	rayTrace.set_vec2("viewBounds", camera.get_view_bounds());
	rayTrace.set_float("time", renderTime);
	rayTrace.set_int("frameCount", frameCount);
	rayTrace.set_int("trianglesCount", trianglesCount);
	rayTrace.set_int("emissionTrianglesCount", emissionTriangles.size());
	rayTrace.set_int("maxBouncesCount", maxBouncesCount);
	rayTrace.set_int("rootId", bvh.rootId);
	rayTrace.set_int("environmentMapId", textures.size() - 1);

	const glm::ivec2 workGroupsCount = imageSize / WORKGROUP_SIZE;
	glDispatchCompute(workGroupsCount.x, workGroupsCount.y, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	screen.use();
	screen.set_float("invFrameCount", Float32(1.0f / frameCount));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	renderManager.draw_quad();
	frameCount++;
}

Void SRaytraceManager::generate_rays(Camera& camera)
{
	const SDisplayManager& displayManager = SDisplayManager::get();
	const glm::ivec2 workGroupsCount = imageSize / WORKGROUP_SIZE;

	camera.set_camera_changed(false);
	frameCount = 1;
	const Float32 theta = glm::radians(camera.get_fov());
	const Float32 h = glm::tan(theta * 0.5f);
	glm::vec2 viewportSize;
	viewportSize.y = 2.0f * h;
	viewportSize.x = viewportSize.y * displayManager.get_aspect_ratio();

	const glm::vec3 viewportU = Float32(viewportSize.x) * camera.get_right();
	const glm::vec3 viewportV = Float32(viewportSize.y) * camera.get_up();

	pixelDeltaU = viewportU / Float32(imageSize.x);
	pixelDeltaV = viewportV / Float32(imageSize.y);
	originPixel = camera.get_position() + camera.get_forward() + (pixelDeltaU - viewportU + pixelDeltaV - viewportV) * 0.5f;

	rayGeneration.use();
	rayGeneration.set_vec3("cameraPosition", camera.get_position());
	rayGeneration.set_vec3("originPixel", originPixel);
	rayGeneration.set_vec3("pixelDeltaU", pixelDeltaU);
	rayGeneration.set_vec3("pixelDeltaV", pixelDeltaV);
	rayGeneration.set_ivec2("imageSize", imageSize);
	glDispatchCompute(workGroupsCount.x, workGroupsCount.y, 1);
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
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

Int32 SRaytraceManager::get_frame_count() const
{
	return frameCount;
}

glm::vec3 SRaytraceManager::get_background_color() const
{
	return backgroundColor;
}

Void SRaytraceManager::reload_shaders()
{
	rayTrace.reload();
	screen.reload();
	rayGeneration.reload();
}

Void SRaytraceManager::shutdown()
{
	glDeleteBuffers(8, ssbo);
	glDeleteTextures(1, &screenTexture);
	screen.shutdown();
	rayTrace.shutdown();
	rayGeneration.shutdown();
}

#pragma once
#include <tiny_gltf.h>

template<typename Type>
struct Handle;


namespace std
{
	namespace filesystem
	{
		class path;
	}
}

struct Texture;
struct Material;
struct Model;
struct Mesh;
enum class ETextureType : Int8;

class SResourceManager
{
public:
	const std::string TEXTURES_PATH = "Resources/Textures/";
	const std::string ASSETS_PATH	= "Resources/Assets/";

	SResourceManager(SResourceManager&) = delete;

	static SResourceManager& get();
	Void startup();

	Void load_gltf_asset(const std::string& filePath);

	Void generate_opengl_texture(Texture& texture);
	Void generate_opengl_model(Model& model);

	Handle<Model>    load_model(const std::filesystem::path & filePath, tinygltf::Mesh& gltfMesh, tinygltf::Model& gltfModel);
	Handle<Mesh>     load_mesh(const std::string& meshName, tinygltf::Primitive& primitive, tinygltf::Model& gltfModel);
	Handle<Material> load_material(const std::filesystem::path& assetPath, tinygltf::Material& gltfMaterial, tinygltf::Model& gltfModel);
	Handle<Texture>  load_texture(const std::filesystem::path& filePath, const std::string& textureName, ETextureType type);

	Handle<Material> create_material(const Material& material, const std::string& name);

	Model    &get_model_by_name(const std::string& name);
	Model	 &get_model_by_handle(const Handle<Model> handle);
	Mesh     &get_mesh_by_name(const std::string& name);
	Mesh     &get_mesh_by_handle(const Handle<Mesh> handle);
	Material &get_material_by_name(const std::string& name);
	Material &get_material_by_handle(const Handle<Material> handle);
	Material &get_default_material();
	Texture  &get_texture_by_name(const std::string& name);
	Texture  &get_texture_by_handle(const Handle<Texture> handle);

	[[nodiscard]]
	const Handle<Model>&	get_model_handle_by_name(const std::string& name)	 const;
	[[nodiscard]]
	const Handle<Mesh>&		get_mesh_handle_by_name(const std::string& name)	 const;
	[[nodiscard]]
	const Handle<Material>& get_material_handle_by_name(const std::string& name) const;
	[[nodiscard]]
	const Handle<Texture>&  get_texture_handle_by_name(const std::string& name)  const;

	[[nodiscard]]
	const std::vector<Model>    &get_models()    const;
	[[nodiscard]]
	const std::vector<Mesh>     &get_meshes()    const;
	[[nodiscard]]
	const std::vector<Material> &get_materials() const;
	[[nodiscard]]
	const std::vector<Texture>  &get_textures()  const;

	Void shutdown();

protected:
	Void load_gltf_asset(const std::filesystem::path& filePath);

	template<typename DataType, typename ArrayType>
	Void process_accessor(tinygltf::Model& gltfModel, const tinygltf::Accessor& accessor, std::vector<ArrayType>& outputData)
	{
		const Int32 bufferViewId = accessor.bufferView;

		const tinygltf::BufferView& bufferView = gltfModel.bufferViews[bufferViewId];
		tinygltf::Buffer& bufferData = gltfModel.buffers[bufferView.buffer];
		UInt8* dataBegin = bufferData.data.data() + accessor.byteOffset + bufferView.byteOffset;

		UInt64 stride = bufferView.byteStride;
		if (stride == 0)
		{
			stride = sizeof(DataType);
		}

		outputData.resize(accessor.count);

		for (Int32 i = 0; i < accessor.count; i++)
		{
			outputData[i] = static_cast<ArrayType>(*reinterpret_cast<DataType*>(dataBegin + stride * i));
		}
	}

private:
	SResourceManager() = default;

	std::unordered_map<std::string, Handle<Model>> nameToIdModels;
	std::vector<Model> models;

	std::unordered_map<std::string, Handle<Mesh>> nameToIdMeshes;
	std::vector<Mesh> meshes;

	std::unordered_map<std::string, Handle<Material>> nameToIdMaterials;
	std::vector<Material> materials;

	std::unordered_map<std::string, Handle<Texture>> nameToIdTextures;
	std::vector<Texture> textures;
};


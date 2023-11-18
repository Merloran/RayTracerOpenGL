#pragma once

template<typename Type>
struct Handle;
struct Mesh;
struct Material;


/** It's just set of meshes and materials */
struct Model
{
	std::vector<Handle<Mesh>> meshes;
	std::vector<Handle<Material>> materials;
	std::string directory;
	std::string name;
};
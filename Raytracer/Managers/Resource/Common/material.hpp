#pragma once
#include "handle.hpp"

struct Texture;
struct Mesh;

/** It's just set of textures */
struct Material
{
	// Note: Opacity should be the last texture in this structure and
	//		 there should be no other types between texture handles
	Handle<Texture> albedo			 = Handle<Texture>::sNone;
	Handle<Texture> normal			 = Handle<Texture>::sNone;
	Handle<Texture> roughness		 = Handle<Texture>::sNone;
	Handle<Texture> metalness		 = Handle<Texture>::sNone;
	Handle<Texture> ambientOcclusion = Handle<Texture>::sNone;
	Handle<Texture> emission		 = Handle<Texture>::sNone;
	Handle<Texture> height			 = Handle<Texture>::sNone;
	Handle<Texture> opacity			 = Handle<Texture>::sNone;
	std::string name;
};

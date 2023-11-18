#pragma once

struct Mesh 
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<UInt32> indexes;
	UInt32 gpuIds[3]{ 0, 0, 0 }; // 0 - VAO, 1 - EBO, 2 - VBO
	std::string name;
};
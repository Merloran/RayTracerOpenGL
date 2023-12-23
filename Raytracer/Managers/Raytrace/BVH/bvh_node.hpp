#pragma once

struct BVHNode
{
	glm::vec3 min;
	Int32 leftId;
	glm::vec3 max;
	Int32 rightId;
};
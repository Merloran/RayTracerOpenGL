#pragma once

struct BVHNode
{
	glm::vec3 min;
	Int32 leftId;
	glm::vec3 max;
	Int32 rightId;
	Int32 parentId;
	Int32 nextId;
	Int32 skipId;
	Int32 primitiveId;
};
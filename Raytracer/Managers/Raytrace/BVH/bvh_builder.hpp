#pragma once
#include "bvh_node.hpp"

struct GPUVertex;

class BVHBuilder
{
public:
	Void create_tree(const std::vector<GPUVertex>& vertexes, const std::vector<UInt32>& indexes);
	
	std::vector<BVHNode> hierarchy;
	Int32 rootId;
private:
	Int32 create_hierarchy(const std::vector<Int32>& srcObjects, Int32 begin, Int32 end);
	Void fill_stackless_data(Int32 nodeId, Int32 parentId);
	Void save_tree(std::string path);
	Bool load_tree(std::string path);
	Void min(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, glm::vec3& result);
	Void max(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, glm::vec3& result);
	Void pad(BVHNode& node);
	Int32 rand_int(Int32 min, Int32 max);
};


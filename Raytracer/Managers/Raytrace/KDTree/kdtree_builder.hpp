#pragma once
#include "kdtree_node.hpp"

class KDTreeBuilder
{
public:
	/*
	 * Creates tree of photons where smaller values in given axis are in the left branch
	 * it sorts by axis starts with x axis, next y in next depth and z and it back to x axis
	 */
	Void create_tree(const std::vector<Photon>& data);
	std::vector<KDTreeNode> nodes;

private:
	Int32 create_hierarchy(const std::vector<Int32>& data, Int32 begin, Int32 end, Int32 axis);
	std::vector<Photon> temporaryData;
	Int32 rootId;
};


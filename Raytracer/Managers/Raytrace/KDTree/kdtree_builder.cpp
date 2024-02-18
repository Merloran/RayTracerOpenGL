#include "kdtree_builder.hpp"

Void KDTreeBuilder::create_tree(const std::vector<Photon>& data)
{
    SPDLOG_INFO("Build KDTree of {} photons", data.size());
	nodes.reserve(data.size());
    temporaryData = data;
    std::vector<Int32> temporaryIndexes;
    temporaryIndexes.reserve(data.size());
    for (Int32 i = 0; i < temporaryIndexes.capacity(); ++i)
    {
        temporaryIndexes.emplace_back(i);
    }
    
    spdlog::set_pattern("%v");
    rootId = create_hierarchy(temporaryIndexes, 0, temporaryIndexes.size(), 0);
    spdlog::set_pattern("%+");
    SPDLOG_INFO("Build KDTree complete.");
}

Int32 KDTreeBuilder::create_hierarchy(const std::vector<Int32>& data, Int32 begin, Int32 end, Int32 axis)
{
    const Int32 objectSpan = end - begin;
    std::vector<Int32> objects = data;
    const Int32 nodeId = nodes.size();
    KDTreeNode& node = nodes.emplace_back();
    auto comparator = [&](const Int32 first, const Int32 second)
    {
        return temporaryData[first].position[axis] < temporaryData[second].position[axis];
    };

    std::sort(objects.begin() + begin, objects.begin() + end, comparator);
    const Int32 mid = begin + objectSpan / 2;

    node.data = temporaryData[objects[mid]];

    axis = (axis + 1) % 3;
    if (objectSpan > 2)
    {
        node.leftId  = create_hierarchy(objects, begin, mid, axis);
        node.rightId = create_hierarchy(objects, mid + 1, end, axis);
    }
    else if (objectSpan > 1)
    {
        node.leftId  = create_hierarchy(objects, begin, mid, axis);
        node.rightId = -1;
    } else {
        node.leftId  = -1;
        node.rightId = -1;
    }
    SPDLOG_INFO("Progress: {0:.2f}%", Float32(nodes.size()) / Float32(nodes.capacity()) * 100.0f);

    return nodeId;
}

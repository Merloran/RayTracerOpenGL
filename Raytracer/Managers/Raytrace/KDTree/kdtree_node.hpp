#pragma once
#include "../Common/photon.hpp"

struct KDTreeNode
{
	Photon data;
	Int32 leftId;
	Int32 rightId;
};
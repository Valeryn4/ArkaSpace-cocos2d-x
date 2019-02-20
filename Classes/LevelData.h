#pragma once

#include <cocos2d.h>

class GridNode;
class ContainerNode;

struct LevelData
{
	static bool toLevelFromMap(const cocos2d::ValueMap &map, GridNode *grid);
	static cocos2d::ValueMap toMapFromLevel(GridNode *node);

	static bool loadMapFromFile(const std::string &file, GridNode *grid);
	static bool saveMapToFile(GridNode* grid, const std::string &file);
};

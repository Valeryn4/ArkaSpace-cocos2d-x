#pragma once

#include <cocos2d.h>

class ObjectNode;

class GridNode : public cocos2d::Node 
{
public:
	virtual bool init();
	void setSizeGrid(int colls, int rows);
	inline int getColls() { return coll; }
	inline int getRows() { return row; }

	bool isPositionFree(int col, int row);
	bool addObjectFromGrid(const int col, const int row, ObjectNode* obj);
	ObjectNode *getObjectFromGrid(int col, int row);
	bool removeObjectFromGrid(int col, int row);
	bool removeObjectFromGrid(ObjectNode* obj);

	cocos2d::Rect getSizeBlock();

	//возвращает вектор грида, (обьекты могут повтор€тьс€ на разных позици€х!!!!
	inline const std::vector<ObjectNode*>& getGrid() const { return grid; }

	//¬озвращает список обьектов без повторов
	std::vector<ObjectNode*> getObjectListFromGrid();

	cocos2d::Vec2 getColRowFromPos(const cocos2d::Point &point);
	cocos2d::Point getPositionFromColRow(int coll, int row);

#if defined(_DEBUG)
	void showDebug();
	void hideDebug();
#endif
	CREATE_FUNC(GridNode);

private:
	int coll = 0;
	int row = 0;
	std::vector<ObjectNode*> grid;

#if defined(_DEBUG)
	std::vector<cocos2d::DrawNode*> debugDraw;
	bool inited_debug = false;
#endif

};
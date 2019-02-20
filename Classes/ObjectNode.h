#pragma once

#include <cocos2d.h>


class GridNode;

class ObjectNode : public cocos2d::Node
{
public:

	struct PosGrid
	{
		int col;
		int row;
	};

	struct SizeGrid
	{

		int col_size;
		int row_size;
	};

	virtual bool init();

	inline auto getSizeFromGrid() { return sizeFromGrid; }
	inline auto getPositionFromGrid() { return pointFromGrid; }

	void setPositionFromGrid(int col, int row);
	void setSizeFromGrid(int col_size, int row_size);

	inline void setGrid(GridNode* grid_) { grid = grid_; }
	inline GridNode* getGrid() { return grid; }

	static void removeFromGridAndCleanup(ObjectNode*);
	inline void setFixedPosition(const cocos2d::Point &point)
	{
		this->setPosition(point);
		fixed_position = point;
	}

	inline auto getFixedPosition() { return fixed_position; }

	std::string getTypeObjectId() { return type_id; }

	CREATE_FUNC(ObjectNode);


protected:
	friend class GridNode;
	GridNode *grid;
	SizeGrid sizeFromGrid = { 0, 0 };
	PosGrid pointFromGrid = { 1, 1 };
	cocos2d::Point fixed_position;

	std::string type_id = "object";

};